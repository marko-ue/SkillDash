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

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdDashAbility)

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

	// The player will dash in the direction of the player's forward vector
	const FVector DashDirection = AvatarPawn->GetActorForwardVector();

	// Impulse strength retrieved from data asset, dictates how far the player gets launched
	const float ImpulseStrength = USdDataAsset::Get().GetDashImpulseStrength();

	// Apply the velocity
	const TSharedPtr<FApplyVelocityEffect> DashEffect = MakeShared<FApplyVelocityEffect>();
	DashEffect->VelocityToApply = DashDirection * ImpulseStrength;
	DashEffect->bAdditiveVelocity = false;

	// Apply the dash movement effect
	MoverComp->QueueInstantMovementEffect(DashEffect);

	// Execute the non replicated gameplay cue for the dash
	if (ActorInfo->IsLocallyControlled())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = AvatarPawn->GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(ActorInfo->AvatarActor.Get(), SdGameplayTags::GameplayCue::DashActivation, CueParams);
	}

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
		CooldownDuration = FMath::Max(0.f, CooldownDuration - PlayerPing);
	}

	// Applies the cooldown GE with a SetByCaller
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GetCooldownGameplayEffect()->GetClass(), GetAbilityLevel(), ASC->MakeEffectContext());
	SpecHandle.Data->SetSetByCallerMagnitude(SdGameplayTags::SetByCaller::DashCooldownDuration, CooldownDuration);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(), ASC->GetPredictionKeyForNewAction());
}
