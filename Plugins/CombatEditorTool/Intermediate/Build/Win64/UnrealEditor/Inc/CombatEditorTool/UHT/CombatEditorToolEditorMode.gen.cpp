// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CombatEditorTool/Public/CombatEditorToolEditorMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCombatEditorToolEditorMode() {}

// Begin Cross Module References
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolEditorMode();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolEditorMode_NoRegister();
UNREALED_API UClass* Z_Construct_UClass_UEdMode();
UPackage* Z_Construct_UPackage__Script_CombatEditorTool();
// End Cross Module References

// Begin Class UCombatEditorToolEditorMode
void UCombatEditorToolEditorMode::StaticRegisterNativesUCombatEditorToolEditorMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCombatEditorToolEditorMode);
UClass* Z_Construct_UClass_UCombatEditorToolEditorMode_NoRegister()
{
	return UCombatEditorToolEditorMode::StaticClass();
}
struct Z_Construct_UClass_UCombatEditorToolEditorMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * This class provides an example of how to extend a UEdMode to add some simple tools\n * using the InteractiveTools framework. The various UEdMode input event handlers (see UEdMode.h)\n * forward events to a UEdModeInteractiveToolsContext instance, which\n * has all the logic for interacting with the InputRouter, ToolManager, etc.\n * The functions provided here are the minimum to get started inserting some custom behavior.\n * Take a look at the UEdMode markup for more extensibility options.\n */" },
#endif
		{ "IncludePath", "CombatEditorToolEditorMode.h" },
		{ "ModuleRelativePath", "Public/CombatEditorToolEditorMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This class provides an example of how to extend a UEdMode to add some simple tools\nusing the InteractiveTools framework. The various UEdMode input event handlers (see UEdMode.h)\nforward events to a UEdModeInteractiveToolsContext instance, which\nhas all the logic for interacting with the InputRouter, ToolManager, etc.\nThe functions provided here are the minimum to get started inserting some custom behavior.\nTake a look at the UEdMode markup for more extensibility options." },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCombatEditorToolEditorMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UCombatEditorToolEditorMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UEdMode,
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolEditorMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCombatEditorToolEditorMode_Statics::ClassParams = {
	&UCombatEditorToolEditorMode::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolEditorMode_Statics::Class_MetaDataParams), Z_Construct_UClass_UCombatEditorToolEditorMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCombatEditorToolEditorMode()
{
	if (!Z_Registration_Info_UClass_UCombatEditorToolEditorMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCombatEditorToolEditorMode.OuterSingleton, Z_Construct_UClass_UCombatEditorToolEditorMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCombatEditorToolEditorMode.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UClass* StaticClass<UCombatEditorToolEditorMode>()
{
	return UCombatEditorToolEditorMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCombatEditorToolEditorMode);
// End Class UCombatEditorToolEditorMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatEditorToolEditorMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UCombatEditorToolEditorMode, UCombatEditorToolEditorMode::StaticClass, TEXT("UCombatEditorToolEditorMode"), &Z_Registration_Info_UClass_UCombatEditorToolEditorMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCombatEditorToolEditorMode), 2423923812U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatEditorToolEditorMode_h_1039577888(TEXT("/Script/CombatEditorTool"),
	Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatEditorToolEditorMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatEditorToolEditorMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
