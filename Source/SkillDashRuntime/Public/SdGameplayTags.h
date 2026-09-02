// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

// UE
#include "NativeGameplayTags.h" // UE_DECLARE_GAMEPLAY_TAG_EXTERN

namespace SdGameplayTags
{
	namespace Event
	{
		SKILLDASHRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DashActivated);
	} // namespace Event
	
	namespace Cooldown
	{
		SKILLDASHRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DashCooldown);
	} // namespace Cooldown
	
	namespace SetByCaller
	{
		SKILLDASHRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DashCooldownDuration);
	} // namespace SetByCaller
	
	namespace GameplayCue
	{
		SKILLDASHRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DashActivation)
	} //namespace GameplayCue
} // namespace SdGameplayTags
