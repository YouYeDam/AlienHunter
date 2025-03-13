// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class AlienHunter : ModuleRules
{
	public AlienHunter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTasks", "UMG", "Slate", "SlateCore", 
		"NavigationSystem", "AIModule" });

        PublicIncludePaths.AddRange(new string[] {
			ModuleDirectory,
			Path.Combine(ModuleDirectory, "BTService"),
			Path.Combine(ModuleDirectory, "BTTask"),
			Path.Combine(ModuleDirectory, "DataStruct"),
			Path.Combine(ModuleDirectory, "GameManager"),
			Path.Combine(ModuleDirectory, "GameMode"),
			Path.Combine(ModuleDirectory, "InteractableActor"),
			Path.Combine(ModuleDirectory, "Monster"),
			Path.Combine(ModuleDirectory, "Player"),
			Path.Combine(ModuleDirectory, "Weapon"),
			Path.Combine(ModuleDirectory, "Widget"),

            Path.Combine(ModuleDirectory, "Data")
        });

#if WITH_EDITOR
        PrivateDependencyModuleNames.AddRange(new string[] {
            "UnrealEd", 
            "EditorSubsystem"
        });
#endif

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
