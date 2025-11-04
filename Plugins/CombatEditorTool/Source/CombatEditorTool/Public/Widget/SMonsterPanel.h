// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class FCombatWaveEditorController;
class ABaseCharacter;
struct FMonsterSpawnData;

class SMonsterPanel : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SMonsterPanel) {}
        SLATE_ARGUMENT(TSharedPtr<FCombatWaveEditorController>, Controller)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    TSharedRef<ITableRow> OnGenerateMonsterRow(TSharedPtr<FMonsterSpawnData> Item, const TSharedRef<STableViewBase>& OwnerTable);

    TSharedRef<ITableRow> OnGenerateCurrentMonsterRow(TSharedPtr<FMonsterSpawnData> Item, const TSharedRef<STableViewBase>& OwnerTable);

    void RequestListRefresh();
private:
    TSharedPtr<FCombatWaveEditorController> Controller;

    TArray<TSharedPtr<FMonsterSpawnData>> MonsterList;

    TSharedPtr<SListView<TSharedPtr<FMonsterSpawnData>>> MonsterListView;

    TArray<TSharedPtr<FMonsterSpawnData>> CurrentMonsterList;

    TSharedPtr<SListView<TSharedPtr<FMonsterSpawnData>>>  CurrentMonsterListView;
};
