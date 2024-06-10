// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NetGroupAIEditorTest : ModuleRules
{
	public NetGroupAIEditorTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"});
		//使用slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd",
            "Slate",  
            "SlateCore",
            "EditorStyle",
            "GraphEditor",
            "BlueprintGraph",
            "KismetCompiler",
            "ToolMenus",
            "NetGroupAI"});

        PublicIncludePaths.AddRange(new string[] { "NetGroupAIEditorTest/Public" });
        PrivateIncludePaths.AddRange(new string[] { "NetGroupAIEditorTest/Private" });

        // 如使用在线功能，则取消注释
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //      if (UEBuildConfiguration.bCompileSteamOSS == true)
        //      {
        //          DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //      }
        // }
    }
}
