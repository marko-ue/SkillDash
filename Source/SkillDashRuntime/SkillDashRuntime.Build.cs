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
				, "GameplayAbilities" // Gameplay Ability System (GAS)
				// Bomber modules
				, "MetaCheatManager" // USdCheatExtension
				, "DataAssetsLoader" // Created USdDataAsset
			}
		);

		PrivateDependencyModuleNames.AddRange(new[]
			{
				"CoreUObject", "Engine", "Slate", "SlateCore" // Core
				, "UMG" // UUserWidget creation
				, "GameplayTags" // FGameplayTag
				, "Mover" // Used for dash impulse
				// Bomber modules
				, "Bomber"
				, "GameFeaturePluginsManager" // Used for GfpmUtils
				, "MyUtils"
			}
		);
	}
}