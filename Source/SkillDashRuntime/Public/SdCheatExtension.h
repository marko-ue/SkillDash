// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "MetaCheatManagerExtension.h"
#include "HAL/IConsoleManager.h"
#include "SdCheatExtension.generated.h"

/**
 * Extends cheat manager with SkillDash-related console commands.
 */
UCLASS()
class SKILLDASHRUNTIME_API USdCheatExtension : public UMetaCheatManagerExtension
{
	GENERATED_BODY()

	/*********************************************************************************************
	 * CVars
	 ********************************************************************************************* */
public:
	/** Override the impulse strength of the dash ability, where 0 is minimum. */
	static TAutoConsoleVariable<float> CVarDashImpulseStrength;
	
	/** Override the cooldown of the dash ability, where 0.01 is the minimum. */
	static TAutoConsoleVariable<float> CVarDashCooldownDuration;
};
