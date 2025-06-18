// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInformationWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "Components/TextBlock.h"
#include "Yuwibo/Character/Tank.h"
#include "Yuwibo/Character/Soldier.h"
void UItemInformationWidget::SetData(uint8 ItemID)
{
	if (ItemID <= 0) return;
	
	if (auto GameInstance = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
       if (auto ItemData = GameInstance->GetItemData(ItemID))
       {
           TArray<UTextBlock*>Texts =
           {
               FirstInformation,
               SecondInformation
           };

           int32 TextIndex = 0;

           auto AddTextIfValid = [&](const FString& Label, const auto& Value, auto ConditionFunc)
               {    
                   FString ValueString;

                   if constexpr (std::is_floating_point_v<std::decay_t<decltype(Value)>>)
                   {
                       // 소수점 최대 2자리까지 출력하고 불필요한 0 제거
                       ValueString = FString::SanitizeFloat(Value, 2);  // 예: 0.05
                   }
                   else
                   {
                       ValueString = LexToString(Value);
                   }

                   if (ConditionFunc(Value) && TextIndex < Texts.Num())
                   {
                       FString InfoString = FString::Printf(TEXT("%s: %s"), *Label, *ValueString);
                       Texts[TextIndex++]->SetText(FText::FromString(InfoString));
                   }
               };

           AddTextIfValid(TEXT("아이템 타입"), StaticEnum<ItemType>()->GetDisplayNameTextByValue((int8)ItemData->ItemType).ToString(), [](const FString& Val) { return Val != TEXT("None"); });

           
           if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->IsA(ASoldier::StaticClass()))
           {
               if (ItemData->ItemType == ItemType::Iron);
               else if (ItemData->ItemType == ItemType::Potion) AddTextIfValid(TEXT("회복"), ItemData->Health, [](int Val) { return Val > 0; });
               else AddTextIfValid(TEXT("분해"), ItemData->Disassembly, [](int Val) { return Val > 0.f; });
           }
           else if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->IsA(ATank::StaticClass()))
           {
               if (ItemData->ItemType == ItemType::Iron)  AddTextIfValid(TEXT("회복"), ItemData->Health, [](int Val) { return Val > 0; });
               else if (ItemData->ItemType == ItemType::Potion);
               else AddTextIfValid(TEXT("분해"), ItemData->Disassembly, [](int Val) { return Val > 0.f; });
           }
           else
           {
               // 조건을 만족하는 데이터만 채워 넣기
               AddTextIfValid(TEXT("공격"), ItemData->Attack, [](const double& Val) { return Val > 0.f; });
               AddTextIfValid(TEXT("수비"), ItemData->Defense, [](double Val) { return Val > 0.f; });
               AddTextIfValid(TEXT("회복"), ItemData->Health, [](int Val) { return Val > 0; });
           }
           // 남은 텍스트 블럭은 비우기
           for (; TextIndex < Texts.Num(); ++TextIndex)
           {
               Texts[TextIndex]->SetText(FText::GetEmpty());
           }
       }
		
	}
}
