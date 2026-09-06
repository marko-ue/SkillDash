// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "CoreMinimal.h"
#include "MetaCheatManagerExtension.h"
#include "SdCheatExtension.generated.h"

/**
 * Extends cheat manager with SkillDash-related console commands.
 */
UCLASS()
class SKILLDASHRUNTIME_API USdCheatExtension : public UMetaCheatManagerExtension
{
	GENERATED_BODY()
	
	/*********************************************************************************************
	 * Main methods
	 ********************************************************************************************* */
public:
	/** Disables and enables the SkillDash GFP, effectively resetting it. */
	UFUNCTION(Exec, meta = (CheatName = "ResetSkillDash"))
	void ResetSkillDash();
	
	/*********************************************************************************************
	 * CVars
	 ********************************************************************************************* */
public:
	/** Override the impulse strength of the dash ability, where 0 is minimum. */
	static TAutoConsoleVariable<float> CVarDashImpulseStrength;
	
	/** Override the cooldown of the dash ability, where 0.01 is the minimum (automatically clamped to 0.01 if 0 is put in). */
	static TAutoConsoleVariable<float> CVarDashCooldownDuration;
};
