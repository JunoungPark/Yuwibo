// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatWavePreviewActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Yuwibo/YuwiboGameInstance.h"

ACombatWavePreviewActor::ACombatWavePreviewActor()
{
    PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
#endif
}

void ACombatWavePreviewActor::BeginPlay()
{
    Super::BeginPlay();
}

#if WITH_EDITOR
void ACombatWavePreviewActor::SpawnMonsterPreview(const FString& MonsterID)
{
    UE_LOG(LogTemp, Log, TEXT("PreviewActor::SpawnMonsterPreview %s"), *MonsterID);
    // 실제 스폰은 여기서 MonsterID에 따라 StaticMesh나 Actor 스폰 로직을 추가
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    auto Preview = GetWorld()->SpawnActor<AActor>(LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/BlueprintClass/DataTable/CharacterDataTable.CharacterDataTable'"))->FindRow<FYuwiboCharacterData>(FName(MonsterID),"")->Class.LoadSynchronous(), GetActorLocation(), FRotator::ZeroRotator, Params);
    if (Preview)
    {
        SpawnedPreviewActors.Emplace(Preview);
        UE_LOG(LogTemp, Log, TEXT("Spawned monster preview: %s"), *MonsterID);
    }
}

void ACombatWavePreviewActor::ClearPreview()
{
    UE_LOG(LogTemp, Log, TEXT("PreviewActor::ClearPreview"));
    // 내부적으로 생성한 액터/컴포넌트 제거 로직 작성
    for (AActor* Actor : SpawnedPreviewActors)
    {
        if (Actor)
            Actor->Destroy();
    }
    SpawnedPreviewActors.Empty();
}
#endif