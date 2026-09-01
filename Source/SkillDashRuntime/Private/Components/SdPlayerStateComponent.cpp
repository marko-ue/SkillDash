// Copyright (c) Marko Petric & Yevhenii Selivanov


#include "Components/SdPlayerStateComponent.h"

// Sd
#include "Data/SdDataAsset.h"
#include "SdGameplayTags.h"

// Bomber
#include "GameFramework/BmrPlayerState.h"
#include "Subsystems/GlobalMessageSubsystem.h"

// UE
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdPlayerStateComponent)

// Sets default values for this component's properties
USdPlayerStateComponent::USdPlayerStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Returns Player State of this component
ABmrPlayerState* USdPlayerStateComponent::GetPlayerState() const
{
	return Cast<ABmrPlayerState>(GetOwner());
}

ABmrPlayerState& USdPlayerStateComponent::GetPlayerStateChecked() const
{
	ABmrPlayerState* MyPlayerState = GetPlayerState();
	checkf(MyPlayerState, TEXT("%s: 'MyPlayerState' is null"), *FString(__FUNCTION__));
	return *MyPlayerState;
}

/*********************************************************************************************
 * Main methods
 ********************************************************************************************* */

// Gives the dash ability to the ASC
void USdPlayerStateComponent::GiveDashAbility()
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	UAbilitySystemComponent& ASC = GetPlayerStateChecked().GetAbilitySystemComponentChecked();
	const FGameplayAbilitySpec AbilitySpec(USdDataAsset::Get().GetDashAbilityClass());
	DashAbilityHandle = ASC.GiveAbility(AbilitySpec);
}

// Clears the dash ability from the ASC
void USdPlayerStateComponent::ClearDashAbility()
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	if (!DashAbilityHandle.IsValid())
	{
		return;
	}

	UAbilitySystemComponent& ASC = GetPlayerStateChecked().GetAbilitySystemComponentChecked();
	ASC.ClearAbility(DashAbilityHandle);
	DashAbilityHandle = FGameplayAbilitySpecHandle();
}

// Broadcasts the dash ability activation event when input is started
void USdPlayerStateComponent::OnDashInputStarted()
{
	FGameplayEventData EventData;
	EventData.EventTag = SdGameplayTags::Event::DashActivated;
	EventData.Instigator = GetPlayerStateChecked().GetPawn();
	UGlobalMessageSubsystem::BroadcastGlobalMessage(EventData);
}

/*********************************************************************************************
 * Overrides
 ********************************************************************************************* */

// Called when the game starts
void USdPlayerStateComponent::BeginPlay()
{
	Super::BeginPlay();
	GiveDashAbility();
}

// Called when the component is unregistered, used to clean up resources
void USdPlayerStateComponent::OnUnregister()
{
	ClearDashAbility();
	Super::OnUnregister();
}
