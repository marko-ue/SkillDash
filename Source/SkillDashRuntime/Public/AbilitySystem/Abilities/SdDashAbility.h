// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SdDashAbility.generated.h"

/**
 * 
 */
UCLASS()
class SKILLDASHRUNTIME_API USdDashAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	/** Actually activate ability, do not call this directly. */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
