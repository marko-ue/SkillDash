// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SdDashAbility.generated.h"

/**
 * Handles applying an impulse in the direction of the player forward vector (a dash)
 * Ability is triggered by the SdGameplayTags::Event::DashActivated event
 */
UCLASS()
class SKILLDASHRUNTIME_API USdDashAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	/*********************************************************************************************
	 * Overrides
	 ********************************************************************************************* */
protected:
	/** Actually activate ability, do not call this directly. */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	/** Is overridden to apply cooldown with set by caller tag for dash cooldown duration. */
	void ApplyCooldown(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo) const;
};
