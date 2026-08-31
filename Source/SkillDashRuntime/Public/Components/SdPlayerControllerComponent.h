// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "Components/ActorComponent.h"

#include "SdPlayerControllerComponent.generated.h"

class ABmrPlayerController;
/**
 * Represents the Player Controller in the SkillDash module, where the Owner is Player Controller actor.
 * Is responsible for managing ability inputs.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SKILLDASHRUNTIME_API USdPlayerControllerComponent : public UActorComponent
{
	GENERATED_BODY()

	/*********************************************************************************************
	 * Public functions
	 ********************************************************************************************* */
public:
	/** Default constructor. */
	USdPlayerControllerComponent();
	
	/** Returns Player Controller of this component. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "[SkillDash]")
	ABmrPlayerController* GetPlayerController() const;
	ABmrPlayerController& GetPlayerControllerChecked() const;
	
	/*********************************************************************************************
	 * Main methods
	 ********************************************************************************************* */
protected:
	/** Sets up the input context for SkillDash for the player controller. */
	void SetupDashInputContext();
	
	/** Removes the input context for SkillDash from the player controller. */
	void RemoveDashInputContext();

	/*********************************************************************************************
	 * Overrides
	 ********************************************************************************************* */
protected:
	/** Called when the owning Actor begins play or when the component is created if the Actor has already begun play. */
	virtual void BeginPlay() override;

	/** Clears all transient data created by this component */
	virtual void OnUnregister() override;
};
