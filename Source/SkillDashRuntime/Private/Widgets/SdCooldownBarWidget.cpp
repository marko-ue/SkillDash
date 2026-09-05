// Copyright (c) Marko Petric & Yevhenii Selivanov

#include "Widgets/SdCooldownBarWidget.h"

// Sd
#include "SdGameplayTags.h"
#include "SdUtils.h"
#include "Components/SdPlayerStateComponent.h"

// Bomber
#include "GameFramework/BmrPlayerState.h"

// UE
#include "Components/ProgressBar.h"
#include "AbilitySystemComponent.h"
#include "Data/SdDataAsset.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdCooldownBarWidget)

/*********************************************************************************************
 * Overrides
 ********************************************************************************************* */

// Called after the underlying slate widget is constructed
void USdCooldownBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
    
	if (!ensureMsgf(CooldownProgressBar, TEXT("ASSERT: [%i] %hs:\n'CooldownProgressBar' is not valid!"), __LINE__, __FUNCTION__))
	{
		return;
	}
    
	CooldownProgressBar->SetPercent(1.f);
	SetVisibility(ESlateVisibility::Collapsed);
    
	const USdPlayerStateComponent* PlayerStateComponent = USdUtils::GetPlayerStateComponent();
	if (!ensureMsgf(PlayerStateComponent, TEXT("ASSERT: [%i] %hs:\n'PlayerStateComponent' is not valid!"), __LINE__, __FUNCTION__))
	{
		return;
	}
    
	UAbilitySystemComponent* ASC = &PlayerStateComponent->GetPlayerStateChecked().GetAbilitySystemComponentChecked();
    
	// Listen for Dash cooldown to show or hide the widget
	ASC->RegisterGameplayTagEvent(SdGameplayTags::Cooldown::DashCooldown, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ThisClass::OnCooldownTagChanged);
}

// Called when the cooldown tag for the Dash ability changes (when it goes on/off cooldown)
void USdCooldownBarWidget::OnCooldownTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		// Cooldown started, store start time and cooldown duration from data asset
		CooldownStartTime = GetWorld()->GetTimeSeconds();
		CooldownDuration = USdDataAsset::Get().GetDashCooldownDuration();
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// Cooldown ended - hide widget
		SetVisibility(ESlateVisibility::Collapsed);
		CooldownProgressBar->SetPercent(1.f);
	}
}

// Is executed every tick when widget is enabled
void USdCooldownBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Don't tick if the widget is not visible. The widget will become visible and tick when the Dash ability is actually on cooldown
	if (GetVisibility() == ESlateVisibility::Collapsed)
	{
		return;
	}

	if (!ensureMsgf(CooldownProgressBar, TEXT("ASSERT: [%i] %hs:\n'CooldownProgressBar' is not valid!"), __LINE__, __FUNCTION__))
	{
		return;
	}

	// Don't tick if there's no cooldown set
	if (CooldownDuration <= 0.f)
	{
		return;
	}

	// Elapsed tracks how much of the cooldown passed, and it's used to accurately update the progress bar percent 
	const float Elapsed = GetWorld()->GetTimeSeconds() - CooldownStartTime;
	const float CooldownPercent = FMath::Clamp(1.f - (Elapsed / CooldownDuration), 0.f, 1.f);
	CooldownProgressBar->SetPercent(CooldownPercent);
}
