// Copyright (c) Marko Petric & Yevhenii Selivanov


#include "Components/SdPlayerStateComponent.h"

// Bomber
#include "GameFramework/BmrPlayerState.h"

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
 * Overrides
 ********************************************************************************************* */

void USdPlayerStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USdPlayerStateComponent::OnUnregister()
{
	Super::OnUnregister();
}
