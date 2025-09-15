// Copyright Epic Games, Inc. All Rights Reserved.

using System.Diagnostics;
using System.IO;
using UnrealBuildTool;

public class Yuwibo : ModuleRules
{
	public Yuwibo(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Latest;

        string ProtobufPath = "C:/Users/jungu/Desktop/vcpkg/installed/x64-windows";

        PublicIncludePaths.Add(Path.Combine(ProtobufPath, "include"));
        PublicAdditionalLibraries.Add(Path.Combine(ProtobufPath, "lib", "abseil_dll.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ProtobufPath, "lib", "libprotobuf.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ProtobufPath, "lib", "libprotobuf-lite.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ProtobufPath, "lib", "libprotoc.lib"));

        PublicDelayLoadDLLs.Add("libprotoc.dll");
        PublicDelayLoadDLLs.Add("abseil_dll.dll");

        PublicDefinitions.Add("PROTOBUF_ENABLE_DEBUG_LOGGING_MAY_LEAK_PII=0"); 
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Slate", "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "NavigationSystem", "AIModule", "GameplayTasks", "UMG", "SlateCore", "EngineCameras", "Landscape", "Paper2D", "CoreOnline", "HTTP", "Json", "JsonUtilities", "Sockets"
            //"OnlineServicesInterface", "OnlineServicesCommon", 
             });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        //PublicDelayLoadDLLs.Add("EOSSDK-Win64-Shipping.dll");

        // RuntimeDependencies.Add("$(BinaryOutputDir)/EOSSDK-Win64-Shipping.dll",
        // Path.Combine(EOSPath, "Binaries", "Win64", "EOSSDK-Win64-Shipping.dll"));
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        string OpenSSLLibPath = "C:/Program Files/OpenSSL-Win64/lib/VC/x64";

        if (Target.Configuration == UnrealTargetConfiguration.Debug)
        {
            // 디버그 빌드인 경우, MDd 버전 사용
            PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibPath, "MDd", "libcrypto.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibPath, "MDd", "libssl.lib"));
        }
        else
        {
            // 릴리스 빌드인 경우, MD 버전 사용
            PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibPath, "MD", "libcrypto.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibPath, "MD", "libssl.lib"));
        }
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicIncludePaths.Add(@"C:/Program Files\OpenSSL-Win64/include");
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            PublicAdditionalLibraries.Add("ssl");
            PublicAdditionalLibraries.Add("crypto");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicAdditionalLibraries.Add("ssl");
            PublicAdditionalLibraries.Add("crypto");
        }
        // Define to allow inclusion of OpenSSL headers in code (adjust as required)

        // .proto → .pb.cc/.pb.h 변환
        string protoDir = Path.Combine(ModuleDirectory, "Networking");
        string genDir = Path.Combine(ModuleDirectory, "Networking");
        
        if (!Directory.Exists(genDir))
        {
            Directory.CreateDirectory(genDir);
        }
        
        string protocPath = Path.Combine(ProtobufPath, "tools", "protobuf", "protoc.exe");
        
        foreach (var protoFile in Directory.GetFiles(protoDir, "*.proto"))
        {
            string fileName = Path.GetFileName(protoFile);
            string outCpp = Path.Combine(genDir, Path.GetFileNameWithoutExtension(protoFile) + ".pb.cc");
            string outH = Path.Combine(genDir, Path.GetFileNameWithoutExtension(protoFile) + ".pb.h");
        
            // 이전 생성 파일 삭제
            if (File.Exists(outCpp)) File.Delete(outCpp);
            if (File.Exists(outH)) File.Delete(outH);
        
            // protoc 실행
            var process = new Process();
            process.StartInfo.FileName = protocPath;
            process.StartInfo.Arguments = $"--cpp_out=\"{genDir}\" -I=\"{protoDir}\" \"{protoFile}\"";
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;
            process.Start();
            process.WaitForExit();
            
            if (process.ExitCode != 0)
            {
                throw new BuildException($"protoc failed for {fileName}: {process.StandardError.ReadToEnd()}");
            }
        }
    }
}
