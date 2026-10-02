// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "Abilities/GameplayAbility.h"

#include "SdDashAbility.generated.h"

/**
 * Handles applying an impulse in the direction of the player forward vector if standing still, otherwise in velocity direction
 * Ability is triggered by the SdGameplayTags::Event::DashActivated event
 */
UCLASS()
class SKILLDASHRUNTIME_API USdDashAbility : public UGameplayAbility
{
	GENERATED_BODY()

	/*********************************************************************************************
	 * Main methods
	 ********************************************************************************************* */
protected:
	/** Handles adding and removing the trail cue after a delay, and executing a cue for the dash sound. */
	UFUNCTION(BlueprintCallable, Category = "[SkillDash]")
	void HandleDashCues(const FGameplayAbilityActorInfo& ActorInfo) const;

	/** Applies the i-frames GE that applies another GE for blocking incoming damage during the specified invincibility duration. */
	UFUNCTION(BlueprintCallable, Category = "[SkillDash]")
	void ApplyDashIFrames(const FGameplayAbilityActorInfo& ActorInfo) const;

	/*********************************************************************************************
	 * Overrides
	 ********************************************************************************************* */
protected:
	/** Is overridden to prevent event-based activation if there is no cooldown GE set. */
	virtual bool ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* TriggerEventData) const override;

	/** Actually activate ability, do not call this directly. */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Is overridden to apply cooldown with set by caller tag for dash cooldown duration. */
	virtual void ApplyCooldown(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo) const override;
};
