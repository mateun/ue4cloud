// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class CloudPlugin : ModuleRules
{
	public CloudPlugin(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"CloudPlugin/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"CloudPlugin/Private",
                "CloudPlugin/Private/ThirdParty",
                // ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
            
            
            
        string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
        Log.TraceError("BaseDir: " + BaseDirectory);
        string CurlDirectory = Path.Combine(BaseDirectory, "ThirdParty", "libcurl", Target.Platform.ToString());
        Log.TraceError("CurlDir: " + CurlDirectory);

        // Add library definitions for non-Mac platforms ... can be done a little later :)
        if ((Target.Platform == UnrealTargetPlatform.Win64))
        {
            string full_library_path_curl = Path.Combine(CurlDirectory, "MyLibcurlBuild64Bit.lib");
            PublicAdditionalLibraries.Add(full_library_path_curl);
            //RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(CurlDirectory, "MyLibcurlBuild64Bit.dll")));
            
        } else {
            string full_library_path_curl = Path.Combine(CurlDirectory, "libcurlwrapper.dylib");
            PublicAdditionalLibraries.Add(full_library_path_curl);
        }

            
        
        

	}
}
