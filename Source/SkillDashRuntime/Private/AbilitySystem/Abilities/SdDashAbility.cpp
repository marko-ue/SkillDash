// Copyright (c) Marko Petric & Yevhenii Selivanov

#include "AbilitySystem/Abilities/SdDashAbility.h"

// Sd
#include "Data/SdDataAsset.h"
#include "SdGameplayTags.h"

// Bomber
#include "Actors/BmrPawn.h"
#include "Components/BmrMoverComponent.h"

// UE
#include "AbilitySystemComponent.h"
#include "DefaultMovementSet/InstantMovementEffects/BasicInstantMovementEffects.h"
#include "GameplayCueManager.h"
#include "MyUtilsLibraries/MultiplayerUtilsLibrary.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdDashAbility)

// How long invincibility will last (i-frames). Lasts around the time it takes for the dash velocity to be used up
constexpr float InvincibilityDuration = 0.133f;

/*********************************************************************************************
 * Main methods
 ********************************************************************************************* */

// Handles adding and removing the trail cue after a delay, and executing a cue for the dash sound
void USdDashAbility::HandleDashCues(const FGameplayAbilityActorInfo& ActorInfo) const
{
	UAbilitySystemComponent* ASC = ActorInfo.AbilitySystemComponent.Get();

	// Add the cue, which attaches the trail Niagara effect to the player's root component
	ASC->AddGameplayCue(SdGameplayTags::GameplayCue::DashTrail, ASC->MakeEffectContext());

	// Remove cue after a short delay to allow the trail to be visible and follow the player
	FTimerHandle TrailTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TrailTimerHandle, [ASC]()
	{
		ASC->RemoveGameplayCue(SdGameplayTags::GameplayCue::DashTrail);
	}, 0.25f, false);

	// Execute a non-replicated cue that plays the dash sound
	if (ActorInfo.IsLocallyControlled())
	{
		const FGameplayCueParameters CueParams;
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(ActorInfo.AvatarActor.Get(), SdGameplayTags::GameplayCue::DashActivation, CueParams);
	}
}

// Applies the i-frames GE that applies another GE for blocking incoming damage during the specified invincibility duration
void USdDashAbility::ApplyDashIFrames(const FGameplayAbilityActorInfo& ActorInfo) const
{
	if (ActorInfo.IsNetAuthority())
	{
		if (UAbilitySystemComponent* ASC = ActorInfo.AbilitySystemComponent.Get())
		{
			checkf(USdDataAsset::Get().GetDashIFramesEffectClass(), TEXT("ERROR: [%i] %hs:\n'DashIFramesEffectClass' is null!"), __LINE__, __FUNCTION__);

			const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(USdDataAsset::Get().GetDashIFramesEffectClass(), GetAbilityLevel(), ASC->MakeEffectContext());
			SpecHandle.Data->SetSetByCallerMagnitude(SdGameplayTags::SetByCaller::DashInvincibilityDuration, InvincibilityDuration);
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(), ASC->GetPredictionKeyForNewAction());
		}
	}
}

/*********************************************************************************************
 * Overrides
 ********************************************************************************************* */

// Is overridden to prevent event-based activation if there is no cooldown GE set
bool USdDashAbility::ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* TriggerEventData) const
{
	return Super::ShouldAbilityRespondToEvent(ActorInfo, TriggerEventData)
	       && ensureMsgf(GetCooldownGameplayEffect(), TEXT("ASSERT: [%i] %hs:\n'CooldownGE' is null!"), __LINE__, __FUNCTION__);
}

// Actually activate ability, do not call this directly
void USdDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ABmrPawn* AvatarPawn = Cast<ABmrPawn>(ActorInfo->AvatarActor.Get());
	if (!ensureMsgf(AvatarPawn, TEXT("ASSERT: [%i] %hs:\n'AvatarPawn' is null!"), __LINE__, __FUNCTION__))
	{
		return;
	}

	UBmrMoverComponent* MoverComp = AvatarPawn->GetMoverComponent();
	if (!ensureMsgf(MoverComp, TEXT("ASSERT: [%i] %hs:\n'MoverComp' is null!"), __LINE__, __FUNCTION__))
	{
		return;
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	// Dash in the direction of current velocity if moving, otherwise use forward vector
	const FVector CurrentVelocity = MoverComp->GetVelocity();
	const FVector DashDirection = CurrentVelocity.SizeSquared() > KINDA_SMALL_NUMBER
	                                  ? CurrentVelocity.GetSafeNormal()
	                                  : AvatarPawn->GetActorForwardVector();

	// Impulse strength retrieved from data asset, dictates how far the player gets launched
	const float ImpulseStrength = USdDataAsset::Get().GetDashImpulseStrength();

	// Store the velocity
	const TSharedPtr<FApplyVelocityEffect> DashEffect = MakeShared<FApplyVelocityEffect>();
	DashEffect->VelocityToApply = DashDirection * ImpulseStrength;
	DashEffect->bAdditiveVelocity = false;

	// Apply the dash movement effect with the velocity
	MoverComp->QueueInstantMovementEffect(DashEffect);

	// Apply i-frames, so the player can't take damage for a specified duration
	ApplyDashIFrames(*ActorInfo);

	// Adds and removes the trail cue after a delay, and executes the cue for the dash sound
	HandleDashCues(*ActorInfo);

	K2_EndAbility();
}

// Is overridden to apply cooldown with set by caller tag for dash cooldown duration
void USdDashAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (!ASC || !ASC->HasAuthorityOrPredictionKey(&ActivationInfo))
	{
		return;
	}

	// Get the cooldown duration from this ability's data asset
	float CooldownDuration = USdDataAsset::Get().GetDashCooldownDuration();

	// Compensate for replication delay on server for non-local clients
	if (ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Authority && !ActorInfo->IsLocallyControlled())
	{
		const APawn* AvatarPawn = Cast<APawn>(ASC->GetAvatarActor());
		const float PlayerPing = UMultiplayerUtilsLibrary::GetPlayerPingSeconds(AvatarPawn);
		CooldownDuration = FMath::Max(KINDA_SMALL_NUMBER, CooldownDuration - PlayerPing);
	}

	// Applies the cooldown GE with a SetByCaller
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GetCooldownGameplayEffect()->GetClass(), GetAbilityLevel(), ASC->MakeEffectContext());
	SpecHandle.Data->SetSetByCallerMagnitude(SdGameplayTags::SetByCaller::DashCooldownDuration, CooldownDuration);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(), ASC->GetPredictionKeyForNewAction());
}
