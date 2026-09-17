// Copyright (c)  Marko Petric & Yevhenii Selivanov

#include "Data/SdDataAsset.h"

// Sd
#include "SdCheatExtension.h"

// Bomber
#include "DalSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdDataAsset)

// Returns this Data Asset, is checked and wil crash if can't be obtained, e.g: when is not set
const USdDataAsset& USdDataAsset::Get()
{
	return UDalSubsystem::GetDataAssetChecked<ThisClass>();
}

// Returns impulse strength from the CVar if set, otherwise default value from data asset
float USdDataAsset::GetDashImpulseStrength() const
{
#if !UE_BUILD_SHIPPING
	const float CVarImpulseStrength = USdCheatExtension::CVarDashImpulseStrength.GetValueOnAnyThread();
	if (CVarImpulseStrength >= 0.f)
	{
		return CVarImpulseStrength;
	}
#endif // !UE_BUILD_SHIPPING

	return DashImpulseStrength;
}

// Returns cooldown duration from the CVar if set, otherwise default value from data asset
float USdDataAsset::GetDashCooldownDuration() const
{
#if !UE_BUILD_SHIPPING
	const float CVarCooldownDuration = USdCheatExtension::CVarDashCooldownDuration.GetValueOnAnyThread();
	if (CVarCooldownDuration >= 0.f)
	{
		// Always clamps to a minimum of 0.01, which prevents the cooldown duration from ever being set to 0,
		// because a cooldown of 0 makes the cooldown stay forever
		return FMath::Max(KINDA_SMALL_NUMBER, CVarCooldownDuration);
	}
#endif // !UE_BUILD_SHIPPING

	return DashCooldownDuration;
}
