// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UEAITextureGenerator : ModuleRules
{
	public UEAITextureGenerator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"Slate",
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore", 
				"EditorScriptingUtilities", 
				"Json", 
				"JsonUtilities", 
				"HTTP", 
				"Settings"
			}
			);
	}
}
