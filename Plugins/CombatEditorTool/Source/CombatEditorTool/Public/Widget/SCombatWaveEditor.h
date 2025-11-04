// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FCombatWaveEditorController;
class SWaveListPanel;
class SMonsterPanel;

/**
 * 
 */
class SCombatWaveEditor : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCombatWaveEditor) {}
        SLATE_ARGUMENT(TSharedPtr<FCombatWaveEditorController>, Controller)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    TSharedPtr<FCombatWaveEditorController> Controller;
    TSharedPtr<SWaveListPanel> WaveListPanel;
    TSharedPtr<SMonsterPanel> MonsterPanel;
};
