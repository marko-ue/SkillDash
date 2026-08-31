// Copyright (c) Marko Petric & Yevhenii Selivanov


#include "SdUtils.h"

// SkillDash
#include "Components/SdPlayerStateComponent.h"

// Bomber
#include "GameFramework/BmrPlayerState.h"
#include "UtilityLibraries/BmrBlueprintFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdUtils)

/*********************************************************************************************
 * Object getters
 ********************************************************************************************* */

// Returns Player State Component with a function to broadcast the ability's gameplay event
USdPlayerStateComponent* USdUtils::GetPlayerStateComponent(const UObject* OptionalWorldContext)
{
	const ABmrPlayerState* PlayerState = UBmrBlueprintFunctionLibrary::GetLocalPlayerState(OptionalWorldContext);
	return PlayerState ? PlayerState->FindComponentByClass<USdPlayerStateComponent>() : nullptr;
}
