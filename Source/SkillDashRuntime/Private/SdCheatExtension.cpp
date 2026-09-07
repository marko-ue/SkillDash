// Copyright (c) Marko Petric & Yevhenii Selivanov


#include "SdCheatExtension.h"

// Bomber
#include "GfpmUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdCheatExtension)

/*********************************************************************************************
 * CVars
 ********************************************************************************************* */

// Override the impulse strength of the dash ability
TAutoConsoleVariable<float> USdCheatExtension::CVarDashImpulseStrength(
	TEXT("Bomber.SkillDash.SetDashImpulseStrength"),
	-1.f,
	TEXT("Override dash impulse strength, -1 uses default from data asset"),
	ECVF_Cheat);

// Override the cooldown of the dash ability
TAutoConsoleVariable<float> USdCheatExtension::CVarDashCooldownDuration(
	TEXT("Bomber.SkillDash.SetDashCooldownDuration"),
	-1.f,
	TEXT("Override dash cooldown duration in seconds, -1 uses default from data asset"),
	ECVF_Cheat);