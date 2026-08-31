// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "SdUtils.generated.h"

/**
 * Static helper functions about SkillDash.
 */
UCLASS()
class SKILLDASHRUNTIME_API USdUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/*********************************************************************************************
	 * Object getters
	 ********************************************************************************************* */
public:
	/** Returns the Player State component of SkillDash. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "[SkillDash]", DisplayName = "Get Sd Player State Component", meta = (WorldContext = "OptionalWorldContext", CallableWithoutWorldContext))
	static class USdPlayerStateComponent* GetPlayerStateComponent(const UObject* OptionalWorldContext = nullptr);
};
