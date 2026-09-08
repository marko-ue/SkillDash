// Copyright (c) Marko Petric & Yevhenii Selivanov

#include "Components/SdPlayerStateComponent.h"

// Sd
#include "Data/SdDataAsset.h"
#include "SdGameplayTags.h"

// Bomber
#include "DalSubsystem.h"
#include "GameFramework/BmrPlayerState.h"
#include "Structures/BmrGameplayTags.h"
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

// Gives the dash ability to the owner's ASC
void USdPlayerStateComponent::GiveDashAbility()
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	// Lambda that gives the Dash ability when the data asset becomes valid
	UDalSubsystem::Get().ListenForDataAsset<USdDataAsset>(this, [this](const USdDataAsset& DA)
	{
		UAbilitySystemComponent& ASC = GetPlayerStateChecked().GetAbilitySystemComponentChecked();
		const FGameplayAbilitySpec AbilitySpec(DA.GetDashAbilityClass());
		DashAbilityHandle = ASC.GiveAbility(AbilitySpec);
	});
}

// Clears the dash ability from the owner's ASC
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

// Clears the Dash ability's cooldown from the owner's ASC
void USdPlayerStateComponent::ClearDashCooldown() const
{
	UAbilitySystemComponent* ASC = &GetPlayerStateChecked().GetAbilitySystemComponentChecked();

	FGameplayTagContainer CooldownTags;
	CooldownTags.AddTag(SdGameplayTags::GameplayEffect::DashCooldown);
	ASC->RemoveActiveEffectsWithGrantedTags(CooldownTags);
}

// Returns the Dash ability spec handle
FGameplayAbilitySpecHandle USdPlayerStateComponent::GetDashAbilityHandle() const
{
	return DashAbilityHandle;
}

// Broadcasts the dash ability activation event when input is started
void USdPlayerStateComponent::OnDashInputStarted()
{
	FGameplayEventData EventData;
	EventData.EventTag = SdGameplayTags::Event::DashActivated;
	EventData.Instigator = GetOwner();
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

	// Listen to remove cooldown tag whenever the game state changes
	UGlobalMessageSubsystem::CallOrStartListeningForGlobalMessage(BmrGameplayTags::Event::GameState_Changed, this, &ThisClass::OnGameStateChanged);
}

// Called when the component is unregistered, used to clean up resources
void USdPlayerStateComponent::OnUnregister()
{
	ClearDashCooldown();

	ClearDashAbility();

	Super::OnUnregister();
}

/*********************************************************************************************
 * Events
 ********************************************************************************************* */

// Called when the current game state was changed
void USdPlayerStateComponent::OnGameStateChanged_Implementation(const struct FGameplayEventData& Payload)
{
	ClearDashCooldown();
}
