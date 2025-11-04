// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class FCombatWaveEditorController;
struct FWaveData;

class SWaveListPanel : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SWaveListPanel) {}
        SLATE_ARGUMENT(TSharedPtr<FCombatWaveEditorController>, Controller)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    
    DECLARE_DELEGATE(FSelectWaveDelegate);
    FSelectWaveDelegate SelectWave;

private:

    TSharedRef<ITableRow> OnGenerateWaveRow(TSharedPtr<FWaveData> Item, const TSharedRef<STableViewBase>& OwnerTable);

    void OnSelectWave(TSharedPtr<FWaveData> SelectedItem, ESelectInfo::Type SelectInfo);

    TSharedPtr<FCombatWaveEditorController> Controller;

    TArray<TSharedPtr<FWaveData>> WaveList;

    TSharedPtr<SListView<TSharedPtr<FWaveData>>> WaveListView;

};
