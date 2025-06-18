// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Yuwibo : ModuleRules
{
	public Yuwibo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Slate", "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "NavigationSystem", "AIModule", "GameplayTasks", "UMG", "SlateCore", "EngineCameras", "Landscape", "Paper2D", "OnlineServicesInterface", "CoreOnline", "HTTP", "Json", "JsonUtilities", "OnlineServicesCommon", "Sockets" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        //PublicDelayLoadDLLs.Add("EOSSDK-Win64-Shipping.dll");

       // RuntimeDependencies.Add("$(BinaryOutputDir)/EOSSDK-Win64-Shipping.dll",
           // Path.Combine(EOSPath, "Binaries", "Win64", "EOSSDK-Win64-Shipping.dll"));
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
