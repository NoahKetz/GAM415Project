// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAM415Project : ModuleRules
{
	public GAM415Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
