// Copyright (c) Marko Petric & Yevhenii Selivanov


#include "AbilitySystem/Abilities/SdDashAbility.h"

// Sd
#include "Data/SdDataAsset.h"

// Bomber
#include "Actors/BmrPawn.h"
#include "Components/BmrMoverComponent.h"

// UE
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
	
	const FVector DashDirection = AvatarPawn->GetActorForwardVector();
	const float ImpulseStrength = USdDataAsset::Get().GetDashImpulseStrength();

	const TSharedPtr<FApplyVelocityEffect> DashEffect = MakeShared<FApplyVelocityEffect>();
	DashEffect->VelocityToApply = DashDirection * ImpulseStrength;
	DashEffect->bAdditiveVelocity = false;

	MoverComp->QueueInstantMovementEffect(DashEffect);

	K2_EndAbility();
}
