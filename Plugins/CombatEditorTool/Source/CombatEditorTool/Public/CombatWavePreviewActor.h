// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatWavePreviewActor.generated.h"

UCLASS()
class COMBATEDITORTOOL_API ACombatWavePreviewActor : public AActor
{
	GENERATED_BODY()
private:
#if WITH_EDITORONLY_DATA
    UPROPERTY(Transient)
    TArray<AActor*> SpawnedPreviewActors;
#endif
public:
        ACombatWavePreviewActor();

#if WITH_EDITOR
        UFUNCTION(CallInEditor, Category = "Preview")
        void SpawnMonsterPreview(const FString& MonsterID);

        UFUNCTION(CallInEditor, Category = "Preview")
        void ClearPreview();
#endif

protected:
    virtual void BeginPlay() override;
};
