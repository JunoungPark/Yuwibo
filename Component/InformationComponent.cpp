// Fill out your copyright notice in the Description page of Project Settings.


#include "InformationComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Character/Golem.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "kismet/GameplayStatics.h"
#include "Components/ListView.h"


UInformationComponent::UInformationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UInformationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character->IsLocallyControlled()) AddViewportWidget();
}

void UInformationComponent::AddViewportWidget()
{
	if (InformationWidget) return;

	if (auto IW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/InformationWidgetBlueprint.InformationWidgetBlueprint_C'")))
	{
		if (auto Widget = CreateWidget(GetWorld(), IW))
		{
			InformationWidget = static_cast<UInformationWidget*>(Widget);
			if (InformationWidget)
			{
				SetData();

				InformationWidget->AddToViewport();
			}
		}
	}
}
void UInformationComponent::DestroyWidget()
{
	if (InformationWidget)
	{
		InformationWidget->RemoveFromParent();  // ������ �θ𿡼� ����
		InformationWidget = nullptr;             // ���۷��� �ʱ�ȭ
	}
}
void UInformationComponent::SetData()
{
	if (!Character || !InformationWidget) return;

	if (auto MyGameInstance = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (auto Data = MyGameInstance->GetCharacterData(Character->GetCharacterName()))
		{
			TArray<UObject*> NewInformationData;

			for (auto& i : Data->SkillTexture)
			{
				auto InformationData = NewObject<UInformationEntryWidget>(this, UInformationEntryWidget::StaticClass());

				InformationData->CharacterName = Character->GetCharacterName();
				NewInformationData.Emplace(InformationData);
			}

			InformationWidget->Informations->SetListItems(NewInformationData);
		}

	}
}

void UInformationComponent::OnOffInformation()
{
	if (InformationWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		InformationWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		InformationWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInformationEntryWidget::SetImage(int32 Index)
{
	if (auto Data = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetCharacterData(CharacterName))
	{
		Image->SetBrushFromSoftTexture(Data->SkillTexture[Index].LoadSynchronous());
		Key->SetText(Data->Key[Index]);
		Information->SetText(Data->Information[Index]);
	}
}

void UInformationEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject); 
	
	CharacterName = static_cast<UInformationEntryWidget*>(ListItemObject)->CharacterName;

	SetImage(Cast<UListView>(GetOwningListView())->GetIndexForItem(ListItemObject));
}
