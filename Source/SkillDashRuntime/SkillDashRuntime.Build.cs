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
				, "UMG" // UUserWidget creation
				, "GameplayAbilities", "GameplayTags" // Gameplay Ability System (GAS)
				// Bomber modules
				, "Bomber"
				, "GameFeaturePluginsManager"
				, "MetaCheatManager" // USdCheatExtension
				, "DataAssetsLoader" // Created USdDataAsset
			}
		);

		PrivateDependencyModuleNames.AddRange(new[]
			{
				"CoreUObject", "Engine", "Slate", "SlateCore" // Core
				, "Mover" // Used for dash impulse
				// Bomber modules
				, "MyUtils"
			}
		);
	}
}