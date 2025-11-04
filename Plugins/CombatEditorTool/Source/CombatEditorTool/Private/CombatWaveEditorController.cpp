#include "CombatWaveEditorController.h"
#include "CombatWaveType.h"
#include "Editor.h"
#include "Engine/World.h"
#include "UObject/Package.h"
#include "ObjectTools.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "UObject/SavePackage.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "AssetRegistry/AssetRegistryModule.h"
FCombatWaveEditorController::FCombatWaveEditorController()
{
}

FCombatWaveEditorController::~FCombatWaveEditorController()
{
	if (!LoadedAsset) return;
	for (FWaveData& Wave : LoadedAsset->Waves)
	{
		if (SpawnMonsters.Num())
		{
			for (auto& Character : SpawnMonsters)
			{
				if (Character) Character->Destroy();
			}
			SpawnMonsters.Empty();
			UE_LOG(LogTemp, Log, TEXT("Cleared preview actor"));
		}
	}
}

void FCombatWaveEditorController::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("FCombatWaveEditorController::Initialize"));
	CreateNewDataAsset();
	// 초기화 작업 (옵션)
}

void FCombatWaveEditorController::LoadDataAsset()
{
#if WITH_EDITOR
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform) return;

	TArray<FString> OutFilenames;
	const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

	if (!DesktopPlatform->OpenFileDialog(
		ParentWindowHandle,
		TEXT("Open CombatWave DataAsset"),
		FPaths::ProjectContentDir(),
		TEXT(""),
		TEXT("Unreal Asset (*.uasset)|*.uasset"),
		EFileDialogFlags::None,
		OutFilenames))
	{
		return;
	}

	if (OutFilenames.Num() == 0) return;

	FString FilePath = OutFilenames[0];
	FString PackageName;
	if (!FPackageName::TryConvertFilenameToLongPackageName(FilePath, PackageName))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid path (must be under Content): %s"), *FilePath);
		return;
	}

	FString AssetName = FPaths::GetBaseFilename(FilePath);

	// 기존 에셋 저장 확인
	if (LoadedAsset && LoadedAsset->GetPackage()->IsDirty())
	{
		EAppReturnType::Type Result = FMessageDialog::Open(
			EAppMsgType::YesNoCancel,
			FText::FromString(TEXT("현재 에셋이 수정되었습니다.\n저장하시겠습니까?"))
		);

		if (Result == EAppReturnType::Cancel) return;
		if (Result == EAppReturnType::Yes) SaveDataAsset();
	}

	// ✅ 실제 ObjectPath를 AssetRegistry로부터 구하기
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(*FString::Printf(TEXT("%s.%s"), *PackageName, *AssetName));
	if (!AssetData.IsValid())
	{
		// 이름이 안 맞을 수도 있으므로, 패키지 안의 모든 에셋 검색
		TArray<FAssetData> Assets;
		AssetRegistry.GetAssetsByPackageName(*PackageName, Assets);

		if (Assets.Num() > 0)
		{
			AssetData = Assets[0]; // 패키지 안의 첫 번째 에셋
			UE_LOG(LogTemp, Warning, TEXT("Auto-resolved asset: %s"), *AssetData.GetObjectPathString());
		}
	}

	if (!AssetData.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find asset data for: %s"), *PackageName);
		return;
	}

	// ✅ 올바른 경로로 로드
	UObject* LoadedObject = AssetData.GetAsset();
	if (UBlueprint* BP = Cast<UBlueprint>(LoadedObject))
	{
		LoadedAsset = Cast<UCombatWaveType>(BP->GeneratedClass->GetDefaultObject());
	}
	else
	{
		LoadedAsset = Cast<UCombatWaveType>(LoadedObject);
	}

	if (LoadedAsset)
	{
		UE_LOG(LogTemp, Log, TEXT("Loaded DataAsset successfully: %s"), *AssetData.GetObjectPathString());
		SelectWave = 0;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load DataAsset: %s"), *AssetData.GetObjectPathString());
	}
#endif

}

void FCombatWaveEditorController::CreateNewDataAsset()
{
#if WITH_EDITOR
	// 간단한 예시: 메모리에서 새로운 UCombatWaveDataAsset 생성 (패키지에 저장은 별도 구현 필요)
	LoadedAsset = NewObject<UCombatWaveType>(GetTransientPackage(), NAME_None);
	if (LoadedAsset)
	{
		UE_LOG(LogTemp, Log, TEXT("Created transient CombatWaveDataAsset"));
	}
#endif
}

void FCombatWaveEditorController::SaveDataAsset()
{
#if WITH_EDITOR
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("No LoadedAsset to save."));
		return;
	}

	UPackage* ExistingPackage = LoadedAsset->GetPackage();
	const FString ExistingPackageName = ExistingPackage ? ExistingPackage->GetName() : TEXT("");

	bool bIsTransient =
		ExistingPackage == GetTransientPackage() ||
		ExistingPackageName.IsEmpty() ||
		ExistingPackageName.StartsWith(TEXT("/Temp/")) ||
		ExistingPackageName.StartsWith(TEXT("/Engine/Transient"));

	FString PackageName;
	FString Filename;

	// (1) 기존에 저장된 에셋이면 → 덮어쓰기
	if (!bIsTransient && FPackageName::DoesPackageExist(ExistingPackageName, nullptr, &Filename))
	{
		PackageName = ExistingPackageName;
		UE_LOG(LogTemp, Log, TEXT("Overwriting existing asset: %s"), *Filename);
	}
	else
	{
		// (2) 새로 저장할 에셋이면 → Save File Dialog 열기
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		if (!DesktopPlatform) return;

		TArray<FString> SavePaths;
		const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

		if (!DesktopPlatform->SaveFileDialog(
			ParentWindowHandle,
			TEXT("Save CombatWave DataAsset"),
			FPaths::ProjectContentDir(),
			TEXT("MyWaveDataAsset"),
			TEXT("Unreal Asset (*.uasset)|*.uasset"),
			EFileDialogFlags::None,
			SavePaths))
		{
			return; // 사용자가 취소
		}

		const FString FilePath = SavePaths[0];
		if (!FPackageName::TryConvertFilenameToLongPackageName(FilePath, PackageName))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid package path: %s"), *FilePath);
			return;
		}

		Filename = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	}

	// 🔹 패키지 생성 또는 가져오기
	UPackage* Package = CreatePackage(*PackageName);
	if (!Package)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create or get package: %s"), *PackageName);
		return;
	}

	// 🔹 Transient에 있는 경우 패키지로 이동
	if (LoadedAsset->GetOutermost() != Package)
	{
		LoadedAsset->Rename(*LoadedAsset->GetName(), Package, REN_DontCreateRedirectors);
	}

	LoadedAsset->SetFlags(RF_Public | RF_Standalone);
	Package->MarkPackageDirty();

	// 🔹 저장 실행
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.SaveFlags = SAVE_None;
	SaveArgs.Error = GError;

	if (UPackage::SavePackage(Package, LoadedAsset, *Filename, SaveArgs))
	{
		FAssetRegistryModule::AssetCreated(LoadedAsset);
		UE_LOG(LogTemp, Log, TEXT("Saved asset successfully: %s"), *Filename);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save asset: %s"), *Filename);
	}

#endif
}

TArray<FMonsterSpawnData> FCombatWaveEditorController::GetWaveMonster()
{
	if (!LoadedAsset || !LoadedAsset->Waves.IsValidIndex(SelectWave)) return TArray<FMonsterSpawnData>();

	return LoadedAsset->Waves[SelectWave].Monsters;
}

int32 FCombatWaveEditorController::AddWave()
{
#if WITH_EDITOR
	if (!LoadedAsset)
	{
		CreateNewDataAsset();
	}
	FWaveData NewWave;

	for (int i = 0;; i++)
	{
		bool IsTrue = false;

		NewWave.WaveName = FString::Printf(TEXT("Wave %d"), i);

		for (auto Wave : LoadedAsset->Waves)
		{
			if (Wave.WaveName == NewWave.WaveName)
			{
				IsTrue = true;
				break;
			}
		}
		if (IsTrue) continue;

		break;
	}

	LoadedAsset->Waves.Add(NewWave);
	LoadedAsset->MarkPackageDirty();

	return LoadedAsset->Waves.Num() - 1;
#else
	return INDEX_NONE;
#endif
}

void FCombatWaveEditorController::AddMonsterToWave(const FMonsterSpawnData Monster)
{
#if WITH_EDITOR
	if (!LoadedAsset) return;
	if (!LoadedAsset->Waves.IsValidIndex(SelectWave)) return;

	LoadedAsset->Waves[SelectWave].Monsters.Emplace(Monster);
	LoadedAsset->MarkPackageDirty();

	SpawnPreview();

	UE_LOG(LogTemp, Log, TEXT("Added monster %s to wave %d"), *Monster.MonsterID, SelectWave);
#endif
}

void FCombatWaveEditorController::SetSelectWave(TSharedPtr<FWaveData> SelectedItem)
{
	if (!LoadedAsset || !SelectedItem.IsValid()) return;
	for (int i = 0; i < LoadedAsset->Waves.Num(); i++)
	{
		if (SelectedItem->WaveName == LoadedAsset->Waves[i].WaveName)
		{
			SelectWave = i;
			SpawnPreview();
			break;
		}
	}
}

void FCombatWaveEditorController::SpawnPreview()
{
#if WITH_EDITOR
	if (!LoadedAsset) return;
	if (!LoadedAsset->Waves.Num()) return;

	UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
	if (!World) return;

	// 이미 존재하면 삭제
	ClearPreview();

	FActorSpawnParameters Params;

	for (const FMonsterSpawnData& Monster : LoadedAsset->Waves[SelectWave].Monsters)
	{
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto Preview = World->SpawnActor<ABaseCharacter>(LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/BlueprintClass/DataTable/CharacterDataTable.CharacterDataTable'"))->FindRow<FYuwiboCharacterData>(FName(Monster.MonsterID), "")->Class.LoadSynchronous(), Monster.Location, FRotator::ZeroRotator, Params);

		SpawnMonsters.Emplace(Preview);

		UE_LOG(LogTemp, Log, TEXT("Spawned monster preview: %s"), *Monster.MonsterID);
	}
#endif
}

void FCombatWaveEditorController::ClearPreview()
{
#if WITH_EDITOR
	if (!LoadedAsset) return;
	if (!LoadedAsset->Waves.IsValidIndex(SelectWave)) return;

	auto Wave = LoadedAsset->Waves[SelectWave];

	if (SpawnMonsters.Num())
	{
		for (auto& Character : SpawnMonsters)
		{
			if (Character) Character->Destroy();
		}
		SpawnMonsters.Empty();
		UE_LOG(LogTemp, Log, TEXT("Cleared preview actor"));
	}
#endif
}