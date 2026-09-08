// Copyright (c)  Marko Petric & Yevhenii Selivanov

#include "Data/SdDataAsset.h"

// Bomber
#include "DalSubsystem.h"
#include "SdCheatExtension.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdDataAsset)

// Returns this Data Asset, is checked and wil crash if can't be obtained, e.g: when is not set
const USdDataAsset& USdDataAsset::Get()
{
	return UDalSubsystem::GetDataAssetChecked<ThisClass>();
}

// Returns default value from the data asset of the dash impulse strength
float USdDataAsset::GetDashImpulseStrength() const
{
#if !UE_BUILD_SHIPPING
	const float CvarImpulseStrength = USdCheatExtension::CVarDashImpulseStrength.GetValueOnAnyThread();
	if (CvarImpulseStrength >= 0.f)
	{
		return CvarImpulseStrength;
	}
#endif // !UE_BUILD_SHIPPING

	return DashImpulseStrength;
}

// Returns default value from the data asset of the dash cooldown duration
float USdDataAsset::GetDashCooldownDuration() const
{
#if !UE_BUILD_SHIPPING
	const float CvarCooldownDuration = USdCheatExtension::CVarDashCooldownDuration.GetValueOnAnyThread();
	if (CvarCooldownDuration >= 0.f)
	{
		// Always clamps to a minimum of 0.01, which prevents the cooldown duration from ever being set to 0,
		// because a cooldown of 0 makes the cooldown stay forever
		return FMath::Max(KINDA_SMALL_NUMBER, CvarCooldownDuration);
	}
#endif // !UE_BUILD_SHIPPING

	return DashCooldownDuration;
}
