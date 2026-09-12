// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"

#include "SdCooldownBarWidget.generated.h"

class UProgressBar;

/**
 * Displays the remaining cooldown of the Dash ability via cooldown bar.
 */
UCLASS()
class SKILLDASHRUNTIME_API USdCooldownBarWidget : public UUserWidget
{
	GENERATED_BODY()

	/*********************************************************************************************
	 * Protected properties
	 ********************************************************************************************* */
protected:
	/** Progress bar for showing the remaining cooldown of the Dash ability. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, Category = "[SkillDash]", meta = (BlueprintProtected, BindWidget))
	TObjectPtr<UProgressBar> CooldownProgressBar = nullptr;

	/** Tracks the world time for when the Dash ability cooldown started. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, AdvancedDisplay, Transient, Category = "[SkillDash]", meta = (BlueprintProtected))
	float CooldownStartTime = 0.f;

	/** Holds the cooldown duration of the Dash ability. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, AdvancedDisplay, Transient, Category = "[SkillDash]", meta = (BlueprintProtected))
	float CooldownDuration = 0.f;

	/*********************************************************************************************
	 * Main methods
	 ********************************************************************************************* */
protected:
	/** Initializes the cooldown bar with a full percentage */
	void ResetPercent() const;

	/** Listen for Dash cooldown to show or hide the widget */
	void BindOnCooldownTagChanged();

	/*********************************************************************************************
	 * Overrides
	 ********************************************************************************************* */
protected:
	/** Called after the underlying slate widget is constructed. */
	virtual void NativeConstruct() override;

	/** Is executed every tick when widget is enabled. */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Called when the widget is removed from the viewport. */
	virtual void NativeDestruct() override;

	/** Called when the cooldown tag for the Dash ability changes (when it goes on/off cooldown) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "[SkillDash]", meta = (BlueprintProtected))
	void OnCooldownTagChanged(FGameplayTag Tag, int32 NewCount);

	/*********************************************************************************************
	 * Events
	 ********************************************************************************************* */
protected:
	/** Called when the local player state is initialized and its assigned character is ready. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "[SkillDash]", meta = (BlueprintProtected))
	void OnLocalPlayerStateReady(const struct FGameplayEventData& Payload);
};
