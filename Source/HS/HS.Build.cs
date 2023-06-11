// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HS : ModuleRules
{
	public HS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "HS.h";
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"TargetingSystem",
			"AIModule",
			"UMG",
		});

		PrivateIncludePaths.AddRange(new string[] { "HS" });
	}
}