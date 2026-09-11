// Copyright (c) Marko Petric & Yevhenii Selivanov

#include "Widgets/SdCooldownBarWidget.h"

// Sd
#include "Components/SdPlayerStateComponent.h"
#include "Data/SdDataAsset.h"
#include "SdGameplayTags.h"
#include "SdUtils.h"

// Bomber
#include "GameFramework/BmrPlayerState.h"
#include "Structures/BmrGameplayTags.h"
#include "Subsystems/GlobalMessageSubsystem.h"

// UE
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdCooldownBarWidget)

/*********************************************************************************************
 * Main methods
 ********************************************************************************************* */

// Initializes the cooldown bar with a full percentage
void USdCooldownBarWidget::ResetPercent() const
{
	if (!ensureMsgf(CooldownProgressBar, TEXT("ASSERT: [%i] %hs:\n'CooldownProgressBar' is not valid!"), __LINE__, __FUNCTION__))
	{
		return;
	}

	CooldownProgressBar->SetPercent(1.f);
}

// Listen for Dash cooldown to show or hide the widget
void USdCooldownBarWidget::BindOnCooldownTagChanged()
{
	const USdPlayerStateComponent* PlayerStateComponent = USdUtils::GetPlayerStateComponent();
	if (!ensureMsgf(PlayerStateComponent, TEXT("ASSERT: [%i] %hs:\n'PlayerStateComponent' is not valid!"), __LINE__, __FUNCTION__))
	{
		return;
	}

	UAbilitySystemComponent* ASC = &PlayerStateComponent->GetPlayerStateChecked().GetAbilitySystemComponentChecked();

	ASC->RegisterGameplayTagEvent(SdGameplayTags::GameplayEffect::DashCooldown, EGameplayTagEventType::NewOrRemoved)
	    .AddUObject(this, &ThisClass::OnCooldownTagChanged);
}

/*********************************************************************************************
 * Overrides
 ********************************************************************************************* */

// Called after the underlying slate widget is constructed
void USdCooldownBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGlobalMessageSubsystem::CallOrStartListeningForGlobalMessage(BmrGameplayTags::Event::Player_LocalPawnReady, this, &ThisClass::OnLocalPlayerStateReady);

	SetVisibility(ESlateVisibility::Collapsed);

	ResetPercent();
}

// Called when the cooldown tag for the Dash ability changes (when it goes on/off cooldown)
void USdCooldownBarWidget::OnCooldownTagChanged_Implementation(const FGameplayTag Tag, int32 NewCount)
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

	if (!CooldownProgressBar)
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

// Called when the widget is removed from the viewport
void USdCooldownBarWidget::NativeDestruct()
{
	const USdPlayerStateComponent* PlayerStateComponent = USdUtils::GetPlayerStateComponent();
	const ABmrPlayerState* PlayerState = PlayerStateComponent ? PlayerStateComponent->GetPlayerState() : nullptr;
	UAbilitySystemComponent* ASC = PlayerState ? PlayerState->GetAbilitySystemComponent() : nullptr;

	if (ASC)
	{
		// Unbind from the gameplay tag event
		ASC->RegisterGameplayTagEvent(
			   SdGameplayTags::GameplayEffect::DashCooldown, EGameplayTagEventType::NewOrRemoved)
			.RemoveAll(this);
	}

	Super::NativeDestruct();
}

/*********************************************************************************************
 * Events
 ********************************************************************************************* */

// Called when the local player state is initialized and its assigned character is ready
void USdCooldownBarWidget::OnLocalPlayerStateReady_Implementation(const FGameplayEventData& Payload)
{
	// Gameplay tag event can now be registered, as the player state and its ASC are both valid
	BindOnCooldownTagChanged();
}
