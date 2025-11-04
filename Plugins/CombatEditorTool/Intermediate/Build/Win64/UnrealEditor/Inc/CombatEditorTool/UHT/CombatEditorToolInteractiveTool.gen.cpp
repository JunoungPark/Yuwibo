// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CombatEditorTool/Private/Tools/CombatEditorToolInteractiveTool.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCombatEditorToolInteractiveTool() {}

// Begin Cross Module References
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolInteractiveTool();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolInteractiveTool_NoRegister();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder_NoRegister();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveTool();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolBuilder();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolPropertySet();
UPackage* Z_Construct_UPackage__Script_CombatEditorTool();
// End Cross Module References

// Begin Class UCombatEditorToolInteractiveToolBuilder
void UCombatEditorToolInteractiveToolBuilder::StaticRegisterNativesUCombatEditorToolInteractiveToolBuilder()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCombatEditorToolInteractiveToolBuilder);
UClass* Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder_NoRegister()
{
	return UCombatEditorToolInteractiveToolBuilder::StaticClass();
}
struct Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Builder for UCombatEditorToolInteractiveTool\n */" },
#endif
		{ "IncludePath", "Tools/CombatEditorToolInteractiveTool.h" },
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Builder for UCombatEditorToolInteractiveTool" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCombatEditorToolInteractiveToolBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder_Statics::ClassParams = {
	&UCombatEditorToolInteractiveToolBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder()
{
	if (!Z_Registration_Info_UClass_UCombatEditorToolInteractiveToolBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCombatEditorToolInteractiveToolBuilder.OuterSingleton, Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCombatEditorToolInteractiveToolBuilder.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UClass* StaticClass<UCombatEditorToolInteractiveToolBuilder>()
{
	return UCombatEditorToolInteractiveToolBuilder::StaticClass();
}
UCombatEditorToolInteractiveToolBuilder::UCombatEditorToolInteractiveToolBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCombatEditorToolInteractiveToolBuilder);
UCombatEditorToolInteractiveToolBuilder::~UCombatEditorToolInteractiveToolBuilder() {}
// End Class UCombatEditorToolInteractiveToolBuilder

// Begin Class UCombatEditorToolInteractiveToolProperties
void UCombatEditorToolInteractiveToolProperties::StaticRegisterNativesUCombatEditorToolInteractiveToolProperties()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCombatEditorToolInteractiveToolProperties);
UClass* Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_NoRegister()
{
	return UCombatEditorToolInteractiveToolProperties::StaticClass();
}
struct Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Property set for the UCombatEditorToolInteractiveTool\n */" },
#endif
		{ "IncludePath", "Tools/CombatEditorToolInteractiveTool.h" },
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Property set for the UCombatEditorToolInteractiveTool" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartPoint_MetaData[] = {
		{ "Category", "Options" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** First point of measurement */" },
#endif
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "First point of measurement" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EndPoint_MetaData[] = {
		{ "Category", "Options" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Second point of measurement */" },
#endif
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Second point of measurement" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Distance_MetaData[] = {
		{ "Category", "Options" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Current distance measurement */" },
#endif
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Current distance measurement" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_StartPoint;
	static const UECodeGen_Private::FStructPropertyParams NewProp_EndPoint;
	static const UECodeGen_Private::FDoublePropertyParams NewProp_Distance;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCombatEditorToolInteractiveToolProperties>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::NewProp_StartPoint = { "StartPoint", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCombatEditorToolInteractiveToolProperties, StartPoint), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartPoint_MetaData), NewProp_StartPoint_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::NewProp_EndPoint = { "EndPoint", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCombatEditorToolInteractiveToolProperties, EndPoint), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EndPoint_MetaData), NewProp_EndPoint_MetaData) };
const UECodeGen_Private::FDoublePropertyParams Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::NewProp_Distance = { "Distance", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCombatEditorToolInteractiveToolProperties, Distance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Distance_MetaData), NewProp_Distance_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::NewProp_StartPoint,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::NewProp_EndPoint,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::NewProp_Distance,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolPropertySet,
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::ClassParams = {
	&UCombatEditorToolInteractiveToolProperties::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::Class_MetaDataParams), Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties()
{
	if (!Z_Registration_Info_UClass_UCombatEditorToolInteractiveToolProperties.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCombatEditorToolInteractiveToolProperties.OuterSingleton, Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCombatEditorToolInteractiveToolProperties.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UClass* StaticClass<UCombatEditorToolInteractiveToolProperties>()
{
	return UCombatEditorToolInteractiveToolProperties::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCombatEditorToolInteractiveToolProperties);
UCombatEditorToolInteractiveToolProperties::~UCombatEditorToolInteractiveToolProperties() {}
// End Class UCombatEditorToolInteractiveToolProperties

// Begin Class UCombatEditorToolInteractiveTool
void UCombatEditorToolInteractiveTool::StaticRegisterNativesUCombatEditorToolInteractiveTool()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCombatEditorToolInteractiveTool);
UClass* Z_Construct_UClass_UCombatEditorToolInteractiveTool_NoRegister()
{
	return UCombatEditorToolInteractiveTool::StaticClass();
}
struct Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * UCombatEditorToolInteractiveTool is an example Tool that allows the user to measure the \n * distance between two points. The first point is set by click-dragging the mouse, and\n * the second point is set by shift-click-dragging the mouse.\n */" },
#endif
		{ "IncludePath", "Tools/CombatEditorToolInteractiveTool.h" },
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "UCombatEditorToolInteractiveTool is an example Tool that allows the user to measure the\ndistance between two points. The first point is set by click-dragging the mouse, and\nthe second point is set by shift-click-dragging the mouse." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Properties_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Properties of the tool are stored here */" },
#endif
		{ "ModuleRelativePath", "Private/Tools/CombatEditorToolInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Properties of the tool are stored here" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Properties;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCombatEditorToolInteractiveTool>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::NewProp_Properties = { "Properties", nullptr, (EPropertyFlags)0x0124080000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCombatEditorToolInteractiveTool, Properties), Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Properties_MetaData), NewProp_Properties_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::NewProp_Properties,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveTool,
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::ClassParams = {
	&UCombatEditorToolInteractiveTool::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::Class_MetaDataParams), Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCombatEditorToolInteractiveTool()
{
	if (!Z_Registration_Info_UClass_UCombatEditorToolInteractiveTool.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCombatEditorToolInteractiveTool.OuterSingleton, Z_Construct_UClass_UCombatEditorToolInteractiveTool_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCombatEditorToolInteractiveTool.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UClass* StaticClass<UCombatEditorToolInteractiveTool>()
{
	return UCombatEditorToolInteractiveTool::StaticClass();
}
UCombatEditorToolInteractiveTool::UCombatEditorToolInteractiveTool() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCombatEditorToolInteractiveTool);
UCombatEditorToolInteractiveTool::~UCombatEditorToolInteractiveTool() {}
// End Class UCombatEditorToolInteractiveTool

// Begin Registration
struct Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Private_Tools_CombatEditorToolInteractiveTool_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UCombatEditorToolInteractiveToolBuilder, UCombatEditorToolInteractiveToolBuilder::StaticClass, TEXT("UCombatEditorToolInteractiveToolBuilder"), &Z_Registration_Info_UClass_UCombatEditorToolInteractiveToolBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCombatEditorToolInteractiveToolBuilder), 2068682676U) },
		{ Z_Construct_UClass_UCombatEditorToolInteractiveToolProperties, UCombatEditorToolInteractiveToolProperties::StaticClass, TEXT("UCombatEditorToolInteractiveToolProperties"), &Z_Registration_Info_UClass_UCombatEditorToolInteractiveToolProperties, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCombatEditorToolInteractiveToolProperties), 3187548936U) },
		{ Z_Construct_UClass_UCombatEditorToolInteractiveTool, UCombatEditorToolInteractiveTool::StaticClass, TEXT("UCombatEditorToolInteractiveTool"), &Z_Registration_Info_UClass_UCombatEditorToolInteractiveTool, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCombatEditorToolInteractiveTool), 462715721U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Private_Tools_CombatEditorToolInteractiveTool_h_4144216384(TEXT("/Script/CombatEditorTool"),
	Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Private_Tools_CombatEditorToolInteractiveTool_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Private_Tools_CombatEditorToolInteractiveTool_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
