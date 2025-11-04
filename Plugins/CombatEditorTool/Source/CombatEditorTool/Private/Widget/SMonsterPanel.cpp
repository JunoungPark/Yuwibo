// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SMonsterPanel.h"
#include "SlateOptMacros.h"
#include "CombatEditorTool/Public/CombatWaveType.h"
#include "CombatWaveEditorController.h"
#include "Widgets/Text/STextBlock.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "Widgets/Input/SNumericEntryBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMonsterPanel::Construct(const FArguments& InArgs)
{
	Controller = InArgs._Controller;

	for (auto& Row : LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/BlueprintClass/DataTable/CharacterDataTable.CharacterDataTable'"))->GetRowNames())
	{
		if (Row == "Empty") continue;
		TSharedPtr<FMonsterSpawnData> NewData = MakeShared<FMonsterSpawnData>();
		NewData->MonsterID = Row.ToString(); // FMonsterSpawnData 구조체의 MonsterID 필드 채움
		MonsterList.Emplace(NewData);
	}

	CurrentMonsterList.Empty();
	for (auto& M : Controller->GetWaveMonster())
	{
		TSharedPtr<FMonsterSpawnData> NewData = MakeShared<FMonsterSpawnData>();
		NewData->MonsterID = M.MonsterID;
		CurrentMonsterList.Emplace(NewData);
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
								.Text(FText::FromString(TEXT("Current Monster List")))

						]

						+ SVerticalBox::Slot().AutoHeight().Padding(4)
						[
							SAssignNew(CurrentMonsterListView, SListView<TSharedPtr<FMonsterSpawnData>>)
								.ListItemsSource(&CurrentMonsterList)
								.OnGenerateRow(this, &SMonsterPanel::OnGenerateCurrentMonsterRow)
								

						]

						// 여기에 세부 편집 위젯 (EditableTextBox, ComboBox 등) 추가 가능]
				]
			+ SHorizontalBox::Slot().AutoWidth().Padding(4)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot().AutoHeight().Padding(4)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Monster List")))

						]
						+ SVerticalBox::Slot().AutoHeight().Padding(4)
						[
							SAssignNew(MonsterListView, SListView<TSharedPtr<FMonsterSpawnData>>)
								.ListItemsSource(&MonsterList)
								.OnGenerateRow(this, &SMonsterPanel::OnGenerateMonsterRow)

						]
				]
		];
}

TSharedRef<ITableRow> SMonsterPanel::OnGenerateCurrentMonsterRow(TSharedPtr<FMonsterSpawnData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FMonsterSpawnData>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock)
						.Text(FText::FromString(Item->MonsterID))
				]
		];
}

TSharedRef<ITableRow> SMonsterPanel::OnGenerateMonsterRow(
	TSharedPtr<FMonsterSpawnData> Item,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FMonsterSpawnData>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock)
						.Text(FText::FromString(Item->MonsterID))
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock)
						.Text(FText::FromString("Location"))
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[SNew(SVectorInputBox)
				.X(Item->Location.X)
				.Y(Item->Location.Y)
				.Z(Item->Location.Z)
				.OnXCommitted_Lambda([Item](float NewX, ETextCommit::Type)
					{
						Item->Location.X = NewX;
					})
				.OnYCommitted_Lambda([Item](float NewY, ETextCommit::Type)
					{
						Item->Location.Y = NewY;
					})
				.OnZCommitted_Lambda([Item](float NewZ, ETextCommit::Type)
					{
						Item->Location.Z = NewZ;
					})
				]
			+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock)
						.Text(FText::FromString("Delay"))
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(SNumericEntryBox<float>)
						.Value_Lambda([Item]() -> TOptional<float>
							{
								return Item.IsValid() ? Item->Delay : 0.0f;
							})
						.OnValueCommitted_Lambda([Item](float NewValue, ETextCommit::Type)
							{
								if (Item.IsValid())
									Item->Delay = NewValue;
							})
						.MinValue(0.0f)      // 최소값
						.MaxValue(60.0f)     // 최대값
						.AllowSpin(true)
				]
			+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock)
						.Text(FText::FromString("Count"))
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(SNumericEntryBox<float>)
						.Value_Lambda([Item]() -> TOptional<float>
							{
								return Item.IsValid() ? Item->Count : 0.0f;
							})
						.OnValueCommitted_Lambda([Item](float NewValue, ETextCommit::Type)
							{
								if (Item.IsValid())
									Item->Count = NewValue;
							})
						.MinValue(0.0f)      // 최소값
						.MaxValue(60.0f)     // 최대값
						.AllowSpin(true)
				]
			+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(SButton)
						.Text(FText::FromString(TEXT("Add")))
						.OnClicked_Lambda([this, Item]() {
						if (Controller.IsValid() && Item.IsValid())
						{
							Controller->AddMonsterToWave(*Item); // 예시: 0번 웨이브에 추가
						}

						RequestListRefresh();

						return FReply::Handled();
							})
				]
		];
}


void SMonsterPanel::RequestListRefresh() {

	if (!Controller.IsValid()) return;

	CurrentMonsterList.Empty();
	for (auto& M : Controller->GetWaveMonster())
	{
		TSharedPtr<FMonsterSpawnData> NewData = MakeShared<FMonsterSpawnData>();
		NewData->MonsterID = M.MonsterID;
		CurrentMonsterList.Emplace(NewData);
	}

	CurrentMonsterListView->RequestListRefresh();

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
