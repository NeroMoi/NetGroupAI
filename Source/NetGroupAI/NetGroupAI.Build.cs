// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NetGroupAI : ModuleRules
{
	public NetGroupAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
		//使用slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

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
