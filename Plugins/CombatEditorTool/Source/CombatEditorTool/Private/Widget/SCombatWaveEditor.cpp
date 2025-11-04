// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SCombatWaveEditor.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Widget/SWaveListPanel.h"
#include "Widget/SMonsterPanel.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCombatWaveEditor::Construct(const FArguments& InArgs)
{
    Controller = InArgs._Controller;

    ChildSlot
        [
            SNew(SBorder)
                .Padding(4)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot().FillWidth(0.33f).Padding(2)
                        [
                            SAssignNew(WaveListPanel, SWaveListPanel)
                                .Controller(Controller)
                        ]

                        + SHorizontalBox::Slot().FillWidth(0.67f).Padding(2)
                        [
                            SAssignNew(MonsterPanel, SMonsterPanel)
                                .Controller(Controller)
                        ]
                ]
        ];

    WaveListPanel->SelectWave.BindLambda([this]() {
        if (!MonsterPanel) return;
        MonsterPanel->RequestListRefresh();
        });
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
