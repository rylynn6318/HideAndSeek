// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HS : ModuleRules
{
	public HS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"Core",
			"CoreUObject",
			"CoreOnline",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"TargetingSystem",
			"AIModule",
			"ModularGameplay",
			"ModularGameplayActors",
			"GameFeatures",
			"SignificanceManager",
			"PhysicsCore",
			"CommonLoadingScreen",
			"ApplicationCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"NetCore",
			"GameplayMessageRuntime",
			"CommonGame",
			"CommonUI",
			"CommonInput",
			"CommonUser",
			"UMG",
			"GameSettings",
			"UIExtension",
			"DTLSHandlerComponent",
			"GameSubtitles",
			"AudioModulation",
			"AudioMixer",
			"DeveloperSettings",
			"Slate",
		});

		PrivateIncludePaths.AddRange(new string[] { "HS" });

		// Generate compile errors if using DrawDebug functions in test/shipping builds.
		PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");

		// Basic setup for External RPC Framework.
		// Functionality within framework will be stripped in shipping to remove vulnerabilities.
		PrivateDependencyModuleNames.Add("ExternalRpcRegistry");
		PrivateDependencyModuleNames.Add("HTTPServer"); // Dependency for ExternalRpcRegistry
		if (Target.Configuration == UnrealTargetConfiguration.Shipping)
		{
			PublicDefinitions.Add("WITH_RPC_REGISTRY=0");
			PublicDefinitions.Add("WITH_HTTPSERVER_LISTENERS=0");
		}
		else
		{
			PublicDefinitions.Add("WITH_RPC_REGISTRY=1");
			PublicDefinitions.Add("WITH_HTTPSERVER_LISTENERS=1");
		}

		SetupGameplayDebuggerSupport(Target);
		SetupIrisSupport(Target);
	}
}