// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CombatEditorTool/Private/Tools/CombatEditorToolSimpleTool.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCombatEditorToolSimpleTool() {}

// Begin Cross Module References
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolSimpleTool();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolSimpleTool_NoRegister();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder_NoRegister();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolSimpleToolProperties();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_NoRegister();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolBuilder();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolPropertySet();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_USingleClickTool();
UPackage* Z_Construct_UPackage__Script_CombatEditorTool();
// End Cross Module References

// Begin Class UCombatEditorToolSimpleToolBuilder
void UCombatEditorToolSimpleToolBuilder::StaticRegisterNativesUCombatEditorToolSimpleToolBuilder()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCombatEditorToolSimpleToolBuilder);
UClass* Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder_NoRegister()
{
	return UCombatEditorToolSimpleToolBuilder::StaticClass();
}
struct Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Builder for UCombatEditorToolSimpleTool\n */" },
#endif
		{ "IncludePath", "Tools/CombatEditorToolSimpleTool.h" },
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolSimpleTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Builder for UCombatEditorToolSimpleTool" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCombatEditorToolSimpleToolBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder_Statics::ClassParams = {
	&UCombatEditorToolSimpleToolBuilder::StaticClass,
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
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder()
{
	if (!Z_Registration_Info_UClass_UCombatEditorToolSimpleToolBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCombatEditorToolSimpleToolBuilder.OuterSingleton, Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCombatEditorToolSimpleToolBuilder.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UClass* StaticClass<UCombatEditorToolSimpleToolBuilder>()
{
	return UCombatEditorToolSimpleToolBuilder::StaticClass();
}
UCombatEditorToolSimpleToolBuilder::UCombatEditorToolSimpleToolBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCombatEditorToolSimpleToolBuilder);
UCombatEditorToolSimpleToolBuilder::~UCombatEditorToolSimpleToolBuilder() {}
// End Class UCombatEditorToolSimpleToolBuilder

// Begin Class UCombatEditorToolSimpleToolProperties
void UCombatEditorToolSimpleToolProperties::StaticRegisterNativesUCombatEditorToolSimpleToolProperties()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCombatEditorToolSimpleToolProperties);
UClass* Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_NoRegister()
{
	return UCombatEditorToolSimpleToolProperties::StaticClass();
}
struct Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Settings UObject for UCombatEditorToolSimpleTool. This UClass inherits from UInteractiveToolPropertySet,\n * which provides an OnModified delegate that the Tool will listen to for changes in property values.\n */" },
#endif
		{ "IncludePath", "Tools/CombatEditorToolSimpleTool.h" },
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolSimpleTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Settings UObject for UCombatEditorToolSimpleTool. This UClass inherits from UInteractiveToolPropertySet,\nwhich provides an OnModified delegate that the Tool will listen to for changes in property values." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShowExtendedInfo_MetaData[] = {
		{ "Category", "Options" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** If enabled, dialog should display extended information about the actor clicked on. Otherwise, only basic info will be shown. */" },
#endif
		{ "DisplayName", "Show Extended Info" },
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolSimpleTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If enabled, dialog should display extended information about the actor clicked on. Otherwise, only basic info will be shown." },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_ShowExtendedInfo_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ShowExtendedInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCombatEditorToolSimpleToolProperties>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
void Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::NewProp_ShowExtendedInfo_SetBit(void* Obj)
{
	((UCombatEditorToolSimpleToolProperties*)Obj)->ShowExtendedInfo = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::NewProp_ShowExtendedInfo = { "ShowExtendedInfo", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UCombatEditorToolSimpleToolProperties), &Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::NewProp_ShowExtendedInfo_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShowExtendedInfo_MetaData), NewProp_ShowExtendedInfo_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::NewProp_ShowExtendedInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolPropertySet,
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::ClassParams = {
	&UCombatEditorToolSimpleToolProperties::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::Class_MetaDataParams), Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCombatEditorToolSimpleToolProperties()
{
	if (!Z_Registration_Info_UClass_UCombatEditorToolSimpleToolProperties.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCombatEditorToolSimpleToolProperties.OuterSingleton, Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCombatEditorToolSimpleToolProperties.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UClass* StaticClass<UCombatEditorToolSimpleToolProperties>()
{
	return UCombatEditorToolSimpleToolProperties::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCombatEditorToolSimpleToolProperties);
UCombatEditorToolSimpleToolProperties::~UCombatEditorToolSimpleToolProperties() {}
// End Class UCombatEditorToolSimpleToolProperties

// Begin Class UCombatEditorToolSimpleTool
void UCombatEditorToolSimpleTool::StaticRegisterNativesUCombatEditorToolSimpleTool()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCombatEditorToolSimpleTool);
UClass* Z_Construct_UClass_UCombatEditorToolSimpleTool_NoRegister()
{
	return UCombatEditorToolSimpleTool::StaticClass();
}
struct Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * UCombatEditorToolSimpleTool is an example Tool that opens a message box displaying info about an actor that the user\n * clicks left mouse button. All the action is in the ::OnClicked handler.\n */" },
#endif
		{ "IncludePath", "Tools/CombatEditorToolSimpleTool.h" },
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolSimpleTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "UCombatEditorToolSimpleTool is an example Tool that opens a message box displaying info about an actor that the user\nclicks left mouse button. All the action is in the ::OnClicked handler." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Properties_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolSimpleTool.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Properties;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCombatEditorToolSimpleTool>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::NewProp_Properties = { "Properties", nullptr, (EPropertyFlags)0x0124080000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCombatEditorToolSimpleTool, Properties), Z_Construct_UClass_UCombatEditorToolSimpleToolProperties_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Properties_MetaData), NewProp_Properties_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::NewProp_Properties,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_USingleClickTool,
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::ClassParams = {
	&UCombatEditorToolSimpleTool::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::Class_MetaDataParams), Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCombatEditorToolSimpleTool()
{
	if (!Z_Registration_Info_UClass_UCombatEditorToolSimpleTool.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCombatEditorToolSimpleTool.OuterSingleton, Z_Construct_UClass_UCombatEditorToolSimpleTool_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCombatEditorToolSimpleTool.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UClass* StaticClass<UCombatEditorToolSimpleTool>()
{
	return UCombatEditorToolSimpleTool::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCombatEditorToolSimpleTool);
UCombatEditorToolSimpleTool::~UCombatEditorToolSimpleTool() {}
// End Class UCombatEditorToolSimpleTool

// Begin Registration
struct Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Private_Tools_CombatEditorToolSimpleTool_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UCombatEditorToolSimpleToolBuilder, UCombatEditorToolSimpleToolBuilder::StaticClass, TEXT("UCombatEditorToolSimpleToolBuilder"), &Z_Registration_Info_UClass_UCombatEditorToolSimpleToolBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCombatEditorToolSimpleToolBuilder), 1418654602U) },
		{ Z_Construct_UClass_UCombatEditorToolSimpleToolProperties, UCombatEditorToolSimpleToolProperties::StaticClass, TEXT("UCombatEditorToolSimpleToolProperties"), &Z_Registration_Info_UClass_UCombatEditorToolSimpleToolProperties, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCombatEditorToolSimpleToolProperties), 667702103U) },
		{ Z_Construct_UClass_UCombatEditorToolSimpleTool, UCombatEditorToolSimpleTool::StaticClass, TEXT("UCombatEditorToolSimpleTool"), &Z_Registration_Info_UClass_UCombatEditorToolSimpleTool, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCombatEditorToolSimpleTool), 1368883566U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Private_Tools_CombatEditorToolSimpleTool_h_2750944870(TEXT("/Script/CombatEditorTool"),
	Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Private_Tools_CombatEditorToolSimpleTool_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Private_Tools_CombatEditorToolSimpleTool_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
