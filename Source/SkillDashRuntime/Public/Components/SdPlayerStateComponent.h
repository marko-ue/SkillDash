// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "Components/ActorComponent.h"
#include "GameplayAbilitySpecHandle.h"

#include "SDPlayerStateComponent.generated.h"

/**
 * Represents the Player State in the SkillDash module, where the Owner is Player State actor.
 * Is responsible for granting and removing the Dash ability.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SKILLDASHRUNTIME_API USdPlayerStateComponent : public UActorComponent
{
	GENERATED_BODY()

	/*********************************************************************************************
	 * Public functions
	 ********************************************************************************************* */
public:
	/** Default constructor. */
	USdPlayerStateComponent();

	/** Returns Player State of this component. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "[SkillDash]")
	class ABmrPlayerState* GetPlayerState() const;
	ABmrPlayerState& GetPlayerStateChecked() const;

	/*********************************************************************************************
	 * Main methods
	 ********************************************************************************************* */
public:
	/** Grants the Dash ability to the owner's ASC. */
	UFUNCTION(BlueprintCallable, Category = "[SkillDash]")
	void GiveDashAbility();

	/** Clears the Dash ability from the owner's ASC. */
	UFUNCTION(BlueprintCallable, Category = "[SkillDash]")
	void ClearDashAbility();

	/** Clears the Dash ability's cooldown from the owner's ASC. */
	UFUNCTION(BlueprintCallable, Category = "[SkillDash]")
	void ClearDashCooldown() const;

	/** Broadcasts the dash ability activation event when input is started. */
	UFUNCTION(BlueprintCallable, Category = "[SkillDash]")
	void OnDashInputStarted();
	
	/*********************************************************************************************
	 * Overrides
	 ********************************************************************************************* */
protected:
	/** Called when the owning Actor begins play or when the component is created if the Actor has already begun play. */
	virtual void BeginPlay() override;

	/** Clears all transient data created by this component. */
	virtual void OnUnregister() override;

	/*********************************************************************************************
	 * Events
	 ********************************************************************************************* */
protected:
	/** Called when the current game state was changed. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "[Bomber]", meta = (BlueprintProtected))
	void OnGameStateChanged(const struct FGameplayEventData& Payload);
};
