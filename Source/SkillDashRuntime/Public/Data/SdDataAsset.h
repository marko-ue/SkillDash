// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "DalPrimaryDataAsset.h"

#include "SdDataAsset.generated.h"

/**
 * Contains common data of the SkillDash plugin to be tweaked.
 */
UCLASS()
class SKILLDASHRUNTIME_API USdDataAsset : public UDalPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** Returns this Data Asset, is checked and wil crash if can't be obtained, e.g: when is not set. */
	static const USdDataAsset& Get(const UObject* OptionalWorldContext = nullptr);
	
	/*********************************************************************************************
	 * Input
	 ********************************************************************************************* */
public:
	/** Returns the dash input context. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "[SkillDash]")
	FORCEINLINE class UBmrInputMappingContext* GetDashInputContext() const { return DashInputContext; }

protected:
	/** Input context for the dash ability. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (BlueprintProtected, ShowOnlyInnerProperties))
	TObjectPtr<class UBmrInputMappingContext> DashInputContext = nullptr;

	/*********************************************************************************************
	 * Gameplay Ability System (GAS)
	 ********************************************************************************************* */
public:
	/** Returns the dash ability class. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "[SkillDash]")
	FORCEINLINE TSubclassOf<class UGameplayAbility> GetDashAbilityClass() const { return DashAbilityClass; }

protected:
	/** The dash ability class to grant to the player. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (BlueprintProtected, ShowOnlyInnerProperties))
	TSubclassOf<UGameplayAbility> DashAbilityClass = nullptr;
};
