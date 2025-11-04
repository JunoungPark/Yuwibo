// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SWaveListPanel.h"

#include "SlateOptMacros.h"
#include "CombatWaveEditorController.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "CombatWaveType.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SWaveListPanel::Construct(const FArguments& InArgs)
{
    Controller = InArgs._Controller;

    if (auto Asset = Controller->GetLoadedAsset())
    {
         auto Waves = &Asset->Waves;

         WaveList.Empty();
         for (const FWaveData& W : *Waves)
         {
             WaveList.Emplace(MakeShared<FWaveData>(W)); // 복사해서 shared로 보관
         }
    }

    ChildSlot
        [
            SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth().Padding(4)
                [
                    SNew(SVerticalBox)
                        + SVerticalBox::Slot().AutoHeight().Padding(4)
                        [
                            SNew(STextBlock)
                                .Text(FText::FromString(TEXT("Wave List")))
                        ]
                        +SVerticalBox::Slot().AutoHeight().Padding(4)
                        [
                            SAssignNew(WaveListView ,SListView<TSharedPtr<FWaveData>>)
                                .ListItemsSource(&WaveList)
                                .OnGenerateRow(this, &SWaveListPanel::OnGenerateWaveRow)
                                .OnSelectionChanged(this, &SWaveListPanel::OnSelectWave)
                        ]
                ]
                + SHorizontalBox::Slot().AutoWidth().Padding(4)
                [
                    SNew(SVerticalBox)
                        + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(SButton)
                                .Text(FText::FromString(TEXT("Load DataAsset")))
                                .OnClicked_Lambda([this]() -> FReply {
                                if (Controller.IsValid())
                                {
                                    Controller->LoadDataAsset();
                                }
                                if (auto Asset = Controller->GetLoadedAsset())
                                {
                                    auto Waves = &Asset->Waves;

                                    WaveList.Empty();
                                    for (const FWaveData& W : *Waves)
                                    {
                                        WaveList.Emplace(MakeShared<FWaveData>(W)); // 복사해서 shared로 보관
                                    }
                                }
                                WaveListView->RequestListRefresh();
                                SelectWave.ExecuteIfBound();
                                return FReply::Handled();
                                    })
                        ]
                        + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(SButton)
                                .Text(FText::FromString(TEXT("Save New DataAsset")))
                                .OnClicked_Lambda([this]() -> FReply {
                                if (Controller.IsValid())
                                {
                                    Controller->SaveDataAsset();
                                }
                                return FReply::Handled();
                                    })
                        ]
                    + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(SButton)
                                .Text(FText::FromString(TEXT("Add Wave")))
                                .OnClicked_Lambda([this]() -> FReply {
                                if (Controller.IsValid())
                                {
                                    Controller->AddWave();

                                    if (auto Asset = Controller->GetLoadedAsset())
                                    {
                                        auto Waves = &Asset->Waves;

                                        WaveList.Empty();
                                        for (const FWaveData& W : *Waves)
                                        {
                                            WaveList.Emplace(MakeShared<FWaveData>(W)); // 복사해서 shared로 보관
                                        }
                                    }

                                    WaveListView->RequestListRefresh();

                                }
                                return FReply::Handled();
                                    })
                        ]
                    + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(SButton)
                                .Text(FText::FromString(TEXT("Spawn Preview Actor")))
                                .OnClicked_Lambda([this]() -> FReply {
                                if (Controller.IsValid())
                                {
                                    Controller->SpawnPreview();
                                }
                                return FReply::Handled();
                                    })
                        ]
                    + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(SButton)
                                .Text(FText::FromString(TEXT("Clear Preview Actor")))
                                .OnClicked_Lambda([this]() -> FReply {
                                if (Controller.IsValid())
                                {
                                    Controller->ClearPreview();
                                }
                                return FReply::Handled();
                                    })
                        ]
                ]

            
        ];
}


TSharedRef<ITableRow> SWaveListPanel::OnGenerateWaveRow(TSharedPtr<FWaveData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
    return SNew(STableRow<TSharedPtr<FWaveData>>, OwnerTable)
        [
            SNew(STextBlock)
                .Text(FText::FromString(Item->WaveName))
        ];
}

void SWaveListPanel::OnSelectWave(TSharedPtr<FWaveData> SelectedItem, ESelectInfo::Type SelectInfo)
{
    Controller->SetSelectWave(SelectedItem);
    SelectWave.ExecuteIfBound();
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
