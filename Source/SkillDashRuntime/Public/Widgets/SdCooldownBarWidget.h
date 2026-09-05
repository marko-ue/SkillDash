// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"

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
	UPROPERTY(Transient)
	float CooldownStartTime = 0.f;

	/** Holds the cooldown duration of the Dash ability. */
	UPROPERTY(Transient)
	float CooldownDuration = 0.f;

	/*********************************************************************************************
	 * Overrides
	 ********************************************************************************************* */
protected:
	/** Called after the underlying slate widget is constructed. */
	virtual void NativeConstruct() override;

	/** Is executed every tick when widget is enabled. */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	/** Called when the cooldown tag for the Dash ability changes (when it goes on/off cooldown) */
	void OnCooldownTagChanged(FGameplayTag Tag, int32 NewCount);
};
