// Copyright (c) Marko Petric & Yevhenii Selivanov


#include "AbilitySystem/Abilities/SdDashAbility.h"

// Sd
#include "Data/SdDataAsset.h"
#include "SdGameplayTags.h"

// Bomber
#include "Actors/BmrPawn.h"
#include "Components/BmrMoverComponent.h"

// UE
#include "GameplayCueManager.h"
#include "DefaultMovementSet/InstantMovementEffects/BasicInstantMovementEffects.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdDashAbility)

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

	// Retrieves the cooldown GE set on this ability's CDO
	const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (!ensureMsgf(CooldownGE, TEXT("ASSERT: [%i] %hs:\n'CooldownGE' is null!"), __LINE__, __FUNCTION__))
	{
		return;
	}

	// Applies the cooldown GE with a SetByCaller. The cooldown duration (magnitude) is retrieved from this ability's data asset
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
	SpecHandle.Data->SetSetByCallerMagnitude(SdGameplayTags::SetByCaller::DashCooldownDuration, USdDataAsset::Get().GetDashCooldownDuration());
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	
	FGameplayCueParameters CueParams;
	CueParams.Location = AvatarPawn->GetActorLocation();
	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(ActorInfo->AvatarActor.Get(), SdGameplayTags::GameplayCue::DashActivation, CueParams);
	
	K2_EndAbility();
}
