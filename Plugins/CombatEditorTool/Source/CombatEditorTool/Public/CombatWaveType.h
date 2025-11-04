// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatWaveType.generated.h"

/**
 * 
 */
class ABaseCharacter;

USTRUCT(BlueprintType)
struct FMonsterSpawnData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MonsterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Delay = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Count = 1;
};

USTRUCT(BlueprintType)
struct FWaveData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString WaveName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMonsterSpawnData> Monsters;
};

 UCLASS(BlueprintType)
class UCombatWaveType : public UDataAsset
{
	GENERATED_BODY()
	
	
public:
     UPROPERTY(EditAnywhere, BlueprintReadWrite)
     TArray<FWaveData> Waves;
	
};
