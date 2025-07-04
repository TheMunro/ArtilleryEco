// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using System.Xml.Linq;
using UnrealBuildTool;

public class Barrage : ModuleRules
{
	public Barrage(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(ModuleDirectory,"../JoltPhysics"), // for jolt includes
			}
			);
			
		bEnableExceptions = true;

		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				Path.Combine(ModuleDirectory,"../JoltPhysics") // for jolt includes
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Chaos",
				"JoltPhysics", "GeometryCore", "SkeletonKey", "mimalloc", "LocomoCore" // <- add jolt dependecy here
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Chaos",
				"Slate",
				"SlateCore",
				"JoltPhysics",
				"LocomoCore",
				"SkeletonKey",
				"mimalloc"// <- add jolt dependecy here
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
			
			
		ExternalDependencies.Add(Path.Combine(ModuleDirectory, "../JoltPhysics")); // checks to determine if jolt needs to be rebuilt
	
	

        // JOLT Stuff - needs to match on both sides.
        DefineIt("JPH_CROSS_PLATFORM_DETERMINISTIC");
        DefineIt("JPH_OBJECT_STREAM"); 
        DefineIt("JPH_OBJECT_LAYER_BITS=16");
        DefineIt("JPH_USE_SSE4_2");
        DefineIt("JPH_USE_SSE4_1");
        DefineIt("JPH_USE_LZCNT");
        DefineIt("JPH_USE_F16C");
        DefineIt("JPH_USE_AVX");
        DefineIt("JPH_USE_AVX2");


        var configType = "";

        if (Target.Configuration == UnrealTargetConfiguration.Debug)
        {

            configType = "Debug";
        }
        else if (Target.Configuration == UnrealTargetConfiguration.DebugGame || Target.Configuration == UnrealTargetConfiguration.Development)
        {
            configType = "Release";
        }
        else
        {
            configType = "Distribution";
        }




        var libPath = "";

        // only for win64. but shouldn't be a problem to do it for other platforms, you just need to change the path
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
			// this path is relative and can change a bit, adjust it according to your project structure. you need to point to library what is built by UE4CMake (*.lib / *.a)
			// TODO: replace this. It's a huge maintenance hazard.
            libPath = Path.Combine(ModuleDirectory, "../../../../Intermediate/CMakeTarget/Jolt/build/Jolt/" + configType, "Jolt.lib");
        }


        PublicAdditionalLibraries.Add(libPath);
		
	}

	private void DefineIt(String str)
	{
		PublicDefinitions.Add(str);
		PrivateDefinitions.Add(str);
	}
}
