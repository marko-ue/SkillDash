// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkillDashRuntime : ModuleRules
{
	public SkillDashRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Error;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"MetaCheatManager", // USdCheatExtension
				"GameplayAbilities", 
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// Bomber modules
				"Bomber"
			}
		);
	}
}
