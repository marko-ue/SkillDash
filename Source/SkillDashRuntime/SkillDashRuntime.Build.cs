// Copyright (c) Marko Petric & Yevhenii Selivanov

using UnrealBuildTool;

public class SkillDashRuntime : ModuleRules
{
	public SkillDashRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Error;

		PublicDependencyModuleNames.AddRange(new[]
			{
				"Core"
				, "GameplayAbilities", "GameplayTags" // Gameplay Ability System (GAS)
				// Bomber modules
				, "MetaCheatManager" // USdCheatExtension
				, "DataAssetsLoader" // Created USdDataAsset
			}
		);

		PrivateDependencyModuleNames.AddRange(new[]
			{
				"CoreUObject", "Engine", "Slate", "SlateCore" // Core
				, "Mover" // Used for dash impulse
				// Bomber modules
				, "Bomber"
				, "MyUtils"
			}
		);
	}
}