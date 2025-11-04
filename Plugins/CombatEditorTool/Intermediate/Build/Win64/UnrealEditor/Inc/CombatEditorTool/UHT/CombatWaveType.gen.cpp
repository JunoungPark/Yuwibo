// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CombatEditorTool/Public/CombatWaveType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCombatWaveType() {}

// Begin Cross Module References
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatWaveType();
COMBATEDITORTOOL_API UClass* Z_Construct_UClass_UCombatWaveType_NoRegister();
COMBATEDITORTOOL_API UScriptStruct* Z_Construct_UScriptStruct_FMonsterSpawnData();
COMBATEDITORTOOL_API UScriptStruct* Z_Construct_UScriptStruct_FWaveData();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
ENGINE_API UClass* Z_Construct_UClass_UDataAsset();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTableRowBase();
UPackage* Z_Construct_UPackage__Script_CombatEditorTool();
// End Cross Module References

// Begin ScriptStruct FMonsterSpawnData
static_assert(std::is_polymorphic<FMonsterSpawnData>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FMonsterSpawnData cannot be polymorphic unless super FTableRowBase is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_MonsterSpawnData;
class UScriptStruct* FMonsterSpawnData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_MonsterSpawnData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_MonsterSpawnData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMonsterSpawnData, (UObject*)Z_Construct_UPackage__Script_CombatEditorTool(), TEXT("MonsterSpawnData"));
	}
	return Z_Registration_Info_UScriptStruct_MonsterSpawnData.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UScriptStruct* StaticStruct<FMonsterSpawnData>()
{
	return FMonsterSpawnData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FMonsterSpawnData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MonsterID_MetaData[] = {
		{ "Category", "MonsterSpawnData" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Location_MetaData[] = {
		{ "Category", "MonsterSpawnData" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Delay_MetaData[] = {
		{ "Category", "MonsterSpawnData" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Count_MetaData[] = {
		{ "Category", "MonsterSpawnData" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_MonsterID;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Delay;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Count;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMonsterSpawnData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::NewProp_MonsterID = { "MonsterID", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMonsterSpawnData, MonsterID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MonsterID_MetaData), NewProp_MonsterID_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMonsterSpawnData, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Location_MetaData), NewProp_Location_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::NewProp_Delay = { "Delay", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMonsterSpawnData, Delay), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Delay_MetaData), NewProp_Delay_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::NewProp_Count = { "Count", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMonsterSpawnData, Count), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Count_MetaData), NewProp_Count_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::NewProp_MonsterID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::NewProp_Location,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::NewProp_Delay,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::NewProp_Count,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
	Z_Construct_UScriptStruct_FTableRowBase,
	&NewStructOps,
	"MonsterSpawnData",
	Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::PropPointers),
	sizeof(FMonsterSpawnData),
	alignof(FMonsterSpawnData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMonsterSpawnData()
{
	if (!Z_Registration_Info_UScriptStruct_MonsterSpawnData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_MonsterSpawnData.InnerSingleton, Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_MonsterSpawnData.InnerSingleton;
}
// End ScriptStruct FMonsterSpawnData

// Begin ScriptStruct FWaveData
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_WaveData;
class UScriptStruct* FWaveData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_WaveData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_WaveData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FWaveData, (UObject*)Z_Construct_UPackage__Script_CombatEditorTool(), TEXT("WaveData"));
	}
	return Z_Registration_Info_UScriptStruct_WaveData.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UScriptStruct* StaticStruct<FWaveData>()
{
	return FWaveData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FWaveData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WaveName_MetaData[] = {
		{ "Category", "WaveData" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Monsters_MetaData[] = {
		{ "Category", "WaveData" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_WaveName;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Monsters_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Monsters;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FWaveData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FWaveData_Statics::NewProp_WaveName = { "WaveName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FWaveData, WaveName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WaveName_MetaData), NewProp_WaveName_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FWaveData_Statics::NewProp_Monsters_Inner = { "Monsters", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMonsterSpawnData, METADATA_PARAMS(0, nullptr) }; // 2109144220
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FWaveData_Statics::NewProp_Monsters = { "Monsters", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FWaveData, Monsters), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Monsters_MetaData), NewProp_Monsters_MetaData) }; // 2109144220
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FWaveData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FWaveData_Statics::NewProp_WaveName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FWaveData_Statics::NewProp_Monsters_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FWaveData_Statics::NewProp_Monsters,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FWaveData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FWaveData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
	nullptr,
	&NewStructOps,
	"WaveData",
	Z_Construct_UScriptStruct_FWaveData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FWaveData_Statics::PropPointers),
	sizeof(FWaveData),
	alignof(FWaveData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FWaveData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FWaveData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FWaveData()
{
	if (!Z_Registration_Info_UScriptStruct_WaveData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_WaveData.InnerSingleton, Z_Construct_UScriptStruct_FWaveData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_WaveData.InnerSingleton;
}
// End ScriptStruct FWaveData

// Begin Class UCombatWaveType
void UCombatWaveType::StaticRegisterNativesUCombatWaveType()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCombatWaveType);
UClass* Z_Construct_UClass_UCombatWaveType_NoRegister()
{
	return UCombatWaveType::StaticClass();
}
struct Z_Construct_UClass_UCombatWaveType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "CombatWaveType.h" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Waves_MetaData[] = {
		{ "Category", "CombatWaveType" },
		{ "ModuleRelativePath", "Public/CombatWaveType.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Waves_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Waves;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCombatWaveType>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCombatWaveType_Statics::NewProp_Waves_Inner = { "Waves", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FWaveData, METADATA_PARAMS(0, nullptr) }; // 4051471459
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UCombatWaveType_Statics::NewProp_Waves = { "Waves", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCombatWaveType, Waves), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Waves_MetaData), NewProp_Waves_MetaData) }; // 4051471459
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCombatWaveType_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCombatWaveType_Statics::NewProp_Waves_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCombatWaveType_Statics::NewProp_Waves,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatWaveType_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UCombatWaveType_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UDataAsset,
	(UObject* (*)())Z_Construct_UPackage__Script_CombatEditorTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatWaveType_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCombatWaveType_Statics::ClassParams = {
	&UCombatWaveType::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UCombatWaveType_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UCombatWaveType_Statics::PropPointers),
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCombatWaveType_Statics::Class_MetaDataParams), Z_Construct_UClass_UCombatWaveType_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCombatWaveType()
{
	if (!Z_Registration_Info_UClass_UCombatWaveType.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCombatWaveType.OuterSingleton, Z_Construct_UClass_UCombatWaveType_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCombatWaveType.OuterSingleton;
}
template<> COMBATEDITORTOOL_API UClass* StaticClass<UCombatWaveType>()
{
	return UCombatWaveType::StaticClass();
}
UCombatWaveType::UCombatWaveType(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCombatWaveType);
UCombatWaveType::~UCombatWaveType() {}
// End Class UCombatWaveType

// Begin Registration
struct Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWaveType_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMonsterSpawnData::StaticStruct, Z_Construct_UScriptStruct_FMonsterSpawnData_Statics::NewStructOps, TEXT("MonsterSpawnData"), &Z_Registration_Info_UScriptStruct_MonsterSpawnData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMonsterSpawnData), 2109144220U) },
		{ FWaveData::StaticStruct, Z_Construct_UScriptStruct_FWaveData_Statics::NewStructOps, TEXT("WaveData"), &Z_Registration_Info_UScriptStruct_WaveData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FWaveData), 4051471459U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UCombatWaveType, UCombatWaveType::StaticClass, TEXT("UCombatWaveType"), &Z_Registration_Info_UClass_UCombatWaveType, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCombatWaveType), 4228640903U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWaveType_h_4212930847(TEXT("/Script/CombatEditorTool"),
	Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWaveType_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWaveType_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWaveType_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Documents_Unreal_Projects_Yuwibo_Plugins_CombatEditorTool_Source_CombatEditorTool_Public_CombatWaveType_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
