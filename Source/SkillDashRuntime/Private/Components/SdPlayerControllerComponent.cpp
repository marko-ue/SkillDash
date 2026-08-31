// Copyright (c) Marko Petric & Yevhenii Selivanov


#include "Components/SdPlayerControllerComponent.h"

// Bomber
#include "Controllers/BmrPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdPlayerControllerComponent)

// Sets default values for this component's properties
USdPlayerControllerComponent::USdPlayerControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Returns Player Controller of this component
ABmrPlayerController* USdPlayerControllerComponent::GetPlayerController() const
{
	return Cast<ABmrPlayerController>(GetOwner());
}

ABmrPlayerController& USdPlayerControllerComponent::GetPlayerControllerChecked() const
{
	ABmrPlayerController* MyPlayerController = GetPlayerController();
	checkf(MyPlayerController, TEXT("%s: 'MyPlayerController' is null"), *FString(__FUNCTION__));
	return *MyPlayerController;
}

/*********************************************************************************************
 * Main methods
 ********************************************************************************************* */

// Sets up the input context for SkillDash for the player controller
void USdPlayerControllerComponent::SetupDashInputContext()
{
	//GetPlayerControllerChecked().SetupInputContexts()
}

// Removes the input context for SkillDash from the player controller
void USdPlayerControllerComponent::RemoveDashInputContext()
{
	//GetPlayerControllerChecked().RemoveInputContexts()
}

/*********************************************************************************************
 * Overrides
 ********************************************************************************************* */

// Called when the owning Actor begins play or when the component is created if the Actor has already begun play
void USdPlayerControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupDashInputContext();
}

// Clears all transient data created by this component
void USdPlayerControllerComponent::OnUnregister()
{
	RemoveDashInputContext();
	Super::OnUnregister();
}
