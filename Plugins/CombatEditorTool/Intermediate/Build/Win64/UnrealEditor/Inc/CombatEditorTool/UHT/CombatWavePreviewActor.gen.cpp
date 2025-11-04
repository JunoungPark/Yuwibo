// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CombatEditorTool/Public/CombatWavePreviewActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCombatWavePreviewActor() {}

// Begin Cross Module References
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_ACombatWavePreviewActor();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_ACombatWavePreviewActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AActor();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
UPackage* Z_Construct_UPackage__Script_CombatEditorTool();
// End Cross Module References

// Begin Class ACombatWavePreviewActor Function ClearPreview
#if WITH_EDITOR
struct Z_Construct_UFunction_ACombatWavePreviewActor_ClearPreview_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "Preview" },
		{ "ModuleRelativePath", "Public/CombatWavePreviewActor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ACombatWavePreviewActor_ClearPreview_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ACombatWavePreviewActor, nullptr, "ClearPreview", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x20020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ACombatWavePreviewActor_ClearPreview_Statics::Function_MetaDataParams), Z_Construct_UFunction_ACombatWavePreviewActor_ClearPreview_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_ACombatWavePreviewActor_ClearPreview()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ACombatWavePreviewActor_ClearPreview_Statics::FuncParams);
	}
	return ReturnFunction;
}
#endif // WITH_EDITOR
#if WITH_EDITOR
DEFINE_FUNCTION(ACombatWavePreviewActor::execClearPreview)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ClearPreview();
	P_NATIVE_END;
}
#endif // WITH_EDITOR
// End Class ACombatWavePreviewActor Function ClearPreview

// Begin Class ACombatWavePreviewActor Function SpawnMonsterPreview
#if WITH_EDITOR
struct Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics
{
	struct CombatWavePreviewActor_eventSpawnMonsterPreview_Parms
	{
		FString MonsterID;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "Preview" },
		{ "ModuleRelativePath", "Public/CombatWavePreviewActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MonsterID_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_MonsterID;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::NewProp_MonsterID = { "MonsterID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(CombatWavePreviewActor_eventSpawnMonsterPreview_Parms, MonsterID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MonsterID_MetaData), NewProp_MonsterID_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::NewProp_MonsterID,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ACombatWavePreviewActor, nullptr, "SpawnMonsterPreview", nullptr, nullptr, Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::PropPointers), sizeof(Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::CombatWavePreviewActor_eventSpawnMonsterPreview_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x20020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::Function_MetaDataParams), Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::CombatWavePreviewActor_eventSpawnMonsterPreview_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview_Statics::FuncParams);
	}
	return ReturnFunction;
}
#endif // WITH_EDITOR
#if WITH_EDITOR
DEFINE_FUNCTION(ACombatWavePreviewActor::execSpawnMonsterPreview)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_MonsterID);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SpawnMonsterPreview(Z_Param_MonsterID);
	P_NATIVE_END;
}
#endif // WITH_EDITOR
// End Class ACombatWavePreviewActor Function SpawnMonsterPreview

// Begin Class ACombatWavePreviewActor
void ACombatWavePreviewActor::StaticRegisterNativesACombatWavePreviewActor()
{
#if WITH_EDITOR
	UClass* Class = ACombatWavePreviewActor::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ClearPreview", &ACombatWavePreviewActor::execClearPreview },
		{ "SpawnMonsterPreview", &ACombatWavePreviewActor::execSpawnMonsterPreview },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
#endif // WITH_EDITOR
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ACombatWavePreviewActor);
UClass* Z_Construct_UClass_ACombatWavePreviewActor_NoRegister()
{
	return ACombatWavePreviewActor::StaticClass();
}
struct Z_Construct_UClass_ACombatWavePreviewActor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "CombatWavePreviewActor.h" },
		{ "ModuleRelativePath", "Public/CombatWavePreviewActor.h" },
	};
#if WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SpawnedPreviewActors_MetaData[] = {
		{ "ModuleRelativePath", "Public/CombatWavePreviewActor.h" },
	};
#endif // WITH_EDITORONLY_DATA
#endif // WITH_METADATA
#if WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SpawnedPreviewActors_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_SpawnedPreviewActors;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#endif // WITH_EDITORONLY_DATA
	static UObject* (*const DependentSingletons[])();
#if WITH_EDITOR
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ACombatWavePreviewActor_ClearPreview, "ClearPreview" }, // 3830987972
		{ &Z_Construct_UFunction_ACombatWavePreviewActor_SpawnMonsterPreview, "SpawnMonsterPreview" }, // 2332300802
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
#endif // WITH_EDITOR
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACombatWavePreviewActor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACombatWavePreviewActor_Statics::NewProp_SpawnedPreviewActors_Inner = { "SpawnedPreviewActors", nullptr, (EPropertyFlags)0x0000000800000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ACombatWavePreviewActor_Statics::NewProp_SpawnedPreviewActors = { "SpawnedPreviewActors", nullptr, (EPropertyFlags)0x0040000800002000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACombatWavePreviewActor, SpawnedPreviewActors), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SpawnedPreviewActors_MetaData), NewProp_SpawnedPreviewActors_MetaData) };
#endif // WITH_EDITORONLY_DATA
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ACombatWavePreviewActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACombatWavePreviewActor_Statics::NewProp_SpawnedPreviewActors_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACombatWavePreviewActor_Statics::NewProp_SpawnedPreviewActors,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACombatWavePreviewActor_Statics::PropPointers) < 2048);
#endif // WITH_EDITORONLY_DATA
UObject* (*const Z_Construct_UClass_ACombatWavePreviewActor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACombatWavePreviewActor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ACombatWavePreviewActor_Statics::ClassParams = {
	&ACombatWavePreviewActor::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	IF_WITH_EDITOR(FuncInfo, nullptr),
	IF_WITH_EDITORONLY_DATA(Z_Construct_UClass_ACombatWavePreviewActor_Statics::PropPointers, nullptr),
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	IF_WITH_EDITOR(UE_ARRAY_COUNT(FuncInfo), 0),
	IF_WITH_EDITORONLY_DATA(UE_ARRAY_COUNT(Z_Construct_UClass_ACombatWavePreviewActor_Statics::PropPointers), 0),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACombatWavePreviewActor_Statics::Class_MetaDataParams), Z_Construct_UClass_ACombatWavePreviewActor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ACombatWavePreviewActor()
{
	if (!Z_Registration_Info_UClass_ACombatWavePreviewActor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ACombatWavePreviewActor.OuterSingleton, Z_Construct_UClass_ACombatWavePreviewActor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ACombatWavePreviewActor.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UClass* StaticClass<ACombatWavePreviewActor>()
{
	return ACombatWavePreviewActor::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ACombatWavePreviewActor);
ACombatWavePreviewActor::~ACombatWavePreviewActor() {}
// End Class ACombatWavePreviewActor

// Begin Registration
struct Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWavePreviewActor_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ACombatWavePreviewActor, ACombatWavePreviewActor::StaticClass, TEXT("ACombatWavePreviewActor"), &Z_Registration_Info_UClass_ACombatWavePreviewActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ACombatWavePreviewActor), 607814247U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWavePreviewActor_h_4088846307(TEXT("/Script/CombatEditorTool"),
	Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWavePreviewActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWavePreviewActor_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
