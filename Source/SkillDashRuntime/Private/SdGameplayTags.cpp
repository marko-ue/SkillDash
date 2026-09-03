// Copyright (c) Marko Petric & Yevhenii Selivanov

#include "SdGameplayTags.h"

namespace SdGameplayTags
{
	namespace UI
	{
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Widget_CooldownBar, "UI.Widget.SkillDash.CooldownBar", "Widget tag for the dash ability cooldown bar widget");
	} // namespace UI
	
	namespace Event
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(DashActivated, "Event.SkillDash.DashActivated", "Event that attempts to activate the dash ability");
	} // namespace Event
	
	namespace Cooldown
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(DashCooldown, "Cooldown.SkillDash.DashCooldown", "Tag for the cooldown of the dash ability");
	} // namespace Cooldown
	
	namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(DashCooldownDuration, "SetByCaller.SkillDash.DashCooldownDuration", "SetByCaller tag to set the cooldown duration of the dash ability");
	} // namespace SetByCaller
	
	namespace GameplayCue
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(DashActivation, "GameplayCue.SkillDash.DashActivation", "Local dash SFX played when the dash is activated")
	} //namespace GameplayCue
} // namespace SdGameplayTags
