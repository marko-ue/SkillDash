// Copyright (c) Marko Petric & Yevhenii Selivanov


#include "Components/SdPlayerControllerComponent.h"

// Sd
#include "Data/SdDataAsset.h"

// Bomber
#include "GfpmUtils.h"
#include "Controllers/BmrPlayerController.h"
#include "DataAssets/BmrInputMappingContext.h"
#include "MyUtilsLibraries/InputUtilsLibrary.h"

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
	UBmrInputMappingContext* DashContext = USdDataAsset::Get().GetDashInputContext();
	if (!ensureMsgf(DashContext, TEXT("ASSERT: [%i] %hs:\n'DashContext' is not valid!"), __LINE__, __FUNCTION__))
	{
		return;
	}

	const TArray<UBmrInputMappingContext*> Contexts = {DashContext};
	GetPlayerControllerChecked().SetupInputContexts(Contexts);
}

// Removes the input context for SkillDash from the player controller
void USdPlayerControllerComponent::RemoveDashInputContext()
{
	UBmrInputMappingContext* DashContext = USdDataAsset::Get().GetDashInputContext();
	if (!ensureMsgf(DashContext, TEXT("ASSERT: [%i] %hs:\n'DashContext' is not valid!"), __LINE__, __FUNCTION__))
	{
		return;
	}

	const TArray<UBmrInputMappingContext*> Contexts = {DashContext};
	GetPlayerControllerChecked().RemoveInputContexts(Contexts);
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
	ABmrPlayerController* MyPC = GetPlayerController();

	UBmrInputMappingContext* DashContext = USdDataAsset::Get().GetDashInputContext();
	if (MyPC && DashContext)
	{
		TArray<UInputAction*> ContextInputActions;
		UInputUtilsLibrary::GetAllActionsInContext(MyPC, DashContext, EInputActionInContextState::Any, /*out*/ ContextInputActions);
		UInputUtilsLibrary::UnbindInputActionsInContext(MyPC, DashContext);
		UGfpmUtils::UnloadAssets(ContextInputActions);
		MyPC->RemoveInputContexts({DashContext});
	}

	Super::OnUnregister();
}
