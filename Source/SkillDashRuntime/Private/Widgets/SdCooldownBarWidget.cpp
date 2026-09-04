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
#include "Abilities/GameplayAbility.h"

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
		// Cooldown started - show widget and start updating
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// Cooldown ended - hide widget
		SetVisibility(ESlateVisibility::Collapsed);
		CooldownProgressBar->SetPercent(0.f);
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

	// Get the player state component
    const USdPlayerStateComponent* PlayerStateComponent = USdUtils::GetPlayerStateComponent();
    if (!ensureMsgf(PlayerStateComponent, TEXT("ASSERT: [%i] %hs:\n'PlayerStateComponent' is not valid!"), __LINE__, __FUNCTION__))
    {
       return;
    }

	// Get the player state
    const ABmrPlayerState* PlayerState = PlayerStateComponent->GetPlayerState();
    if (!ensureMsgf(PlayerState, TEXT("ASSERT: [%i] %hs:\n'PlayerState' is not valid!"), __LINE__, __FUNCTION__))
    {
       return;
    }

	// Get the ASC from the player state
    const UAbilitySystemComponent* ASC = &PlayerState->GetAbilitySystemComponentChecked();

	// Get the handle of the Dash ability
    const FGameplayAbilitySpecHandle DashAbilityHandle = PlayerStateComponent->GetDashAbilityHandle();
    if (!DashAbilityHandle.IsValid())
    {
       return;
    }

	// Get the ability spec from the handle of the Dash ability
    const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromHandle(DashAbilityHandle);
    if (!AbilitySpec)
    {
       return;
    }

	// Track the time remaining until the cooldown ends and the cooldown duration
    float TimeRemaining = 0.f;
    float Duration = 0.f;

	// Retrieves the Dash ability's cooldown's time remaining and duration
    AbilitySpec->Ability->GetCooldownTimeRemainingAndDuration(
       DashAbilityHandle, ASC->AbilityActorInfo.Get(), TimeRemaining, Duration);
	
	if (TimeRemaining <= 0.f || Duration <= 0.f)
	{
		return;
	}

	// Update the percent of the progress bar
	const float CooldownPercent = TimeRemaining / Duration;
	CooldownProgressBar->SetPercent(CooldownPercent);
}
