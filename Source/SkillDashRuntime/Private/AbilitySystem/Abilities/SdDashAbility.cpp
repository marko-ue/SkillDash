// Copyright (c) Marko Petric & Yevhenii Selivanov


#include "AbilitySystem/Abilities/SdDashAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdDashAbility)

// Actually activate ability, do not call this directly
void USdDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
