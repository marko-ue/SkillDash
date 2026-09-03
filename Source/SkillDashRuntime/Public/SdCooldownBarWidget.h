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

	/*********************************************************************************************
	 * Protected functions
	 ********************************************************************************************* */
protected:
	/** Called after the underlying slate widget is constructed. */
	virtual void NativeConstruct() override;

	/** Is executed every tick when widget is enabled. */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	/** Called when the cooldown tag for the Dash ability changes (when it goes on/off cooldown) */
	void OnCooldownTagChanged(FGameplayTag Tag, int32 NewCount);
};
