// Copyright (c) Marko Petric & Yevhenii Selivanov

#include "Components/SdPlayerControllerComponent.h"

// Sd
#include "Data/SdDataAsset.h"

// Bomber
#include "Controllers/BmrPlayerController.h"
#include "DalSubsystem.h"
#include "DataAssets/BmrInputMappingContext.h"
#include "GfpmUtils.h"
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
	return GetOwner<ABmrPlayerController>();
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
void USdPlayerControllerComponent::SetupDashInputContext() const
{
	// Lambda that sets up input contexts when the data asset becomes valid
	UDalSubsystem::Get().ListenForDataAsset<USdDataAsset>(this, [this](const USdDataAsset& DA)
	{
		const TArray<const UBmrInputMappingContext*> Contexts = {DA.GetDashInputContext()};
		GetPlayerControllerChecked().SetupInputContexts(Contexts);
	});
}

// Removes the input context for SkillDash from the player controller
void USdPlayerControllerComponent::RemoveDashInputContextAndActions() const
{
	ABmrPlayerController* MyPC = GetPlayerController();

	const USdDataAsset* DataAsset = UDalSubsystem::GetDataAsset<USdDataAsset>();
	UBmrInputMappingContext* DashContext = DataAsset ? DataAsset->GetDashInputContext() : nullptr;
	if (MyPC && DashContext)
	{
		TArray<UInputAction*> ContextInputActions;
		UInputUtilsLibrary::GetAllActionsInContext(MyPC, DashContext, EInputActionInContextState::Any, /*out*/ ContextInputActions);
		UInputUtilsLibrary::UnbindInputActionsInContext(MyPC, DashContext);
		UGfpmUtils::UnloadAssets(ContextInputActions);
		MyPC->RemoveInputContexts({DashContext});
	}
}

/*********************************************************************************************
 * Overrides
 ********************************************************************************************* */

// Called when the owning Actor begins play or when the component is created if the Actor has already begun play
void USdPlayerControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetPlayerControllerChecked().IsLocalController())
	{
		SetupDashInputContext();
	}
}

// Clears all transient data created by this component
void USdPlayerControllerComponent::OnUnregister()
{
	if (GetPlayerControllerChecked().IsLocalController())
	{
		RemoveDashInputContextAndActions();
	}
	
	Super::OnUnregister();
}
