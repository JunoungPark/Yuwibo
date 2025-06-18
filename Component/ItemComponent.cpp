// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "Components/Image.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/TileView.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "HealthComponent.h"
#include "Yuwibo/Character/Tank.h"
#include "Yuwibo/Character/Soldier.h"
#include "Yuwibo/Widget/ItemInformationWidget.h"
#include "Components/ProgressBar.h"

void Potion::Use(ABaseCharacter* Character, FItemData& ItemData)
{
	if(auto Health = Character->GetComponent<UHealthComponent>(Component::Health))
	{
		if (Health->GetMaxHealth() == Health->GetCurrentHealth()) return;

		if (auto Item = Character->GetComponent<UItemComponent>(Component::Item))
		{
			if (Item->IsUsingPotion()) return;

			Item->StartUsePotion(ItemData);

		}
	}
}
//FItem* FItem::Use(ABaseCharacter* Character)
//{
//	switch (Type)
//	{
//	case ItemType::Helmet:
//		if (auto equipment = Character->GetComponent<UEquipmentComponent>(Component::Equipment))
//			return equipment->SetEquipment(static_cast<Equipment*>(this));
//		break;
//	case ItemType::Armor:
//		if (auto equipment = Character->GetComponent<UEquipmentComponent>(Component::Equipment))
//			return equipment->SetEquipment(static_cast<Equipment*>(this));
//		break;
//	case ItemType::Pants:
//		if (auto equipment = Character->GetComponent<UEquipmentComponent>(Component::Equipment))
//			return equipment->SetEquipment(static_cast<Equipment*>(this));
//		break;
//	case ItemType::Boots:
//		if (auto equipment = Character->GetComponent<UEquipmentComponent>(Component::Equipment))
//			return equipment->SetEquipment(static_cast<Equipment*>(this));
//		break;
//	case ItemType::Weapon:
//		if (auto equipment = Character->GetComponent<UEquipmentComponent>(Component::Equipment))
//			return equipment->SetEquipment(static_cast<Equipment*>(this));
//		break;
//		//case ItemType::Potion:
//		//	break;
//	default:
//		break;
//	}
//	return this;
//}

//UTexture2D* UItemHolder::GetItemTexture()
//{
//	
//	if (Item_Texture == nullptr) 
//	{
//		if (auto GameInstance = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
//			Item_Texture = GameInstance->GetItemData(Item->GetItemName())->ItemTexture.LoadSynchronous();
//	}
//	return Item_Texture;
//}

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (Item.ItemID != 0 && InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) 
	{
		if (auto TileView = Cast<UTileView>(GetOwningListView()))
		{
			if (auto ItemComponent = static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetComponent<UItemComponent>(Component::Item))
			{
				ItemComponent->Use(ItemComponent->CheckPlayerItem(TileView), TileView->GetIndexInList(GetListItem()));
			}
		}
		else if (auto ItemComponent = static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetComponent<UItemComponent>(Component::Item))
		{
			if (ItemComponent->CheckEquipmentWidget(this) != EquipmentWidget::None)
				ItemComponent->UnwearEquipment(ItemComponent->CheckEquipmentWidget(this));
		}
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//return FReply(UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply);
}

void UItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (auto ItemComponent = static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetComponent<UItemComponent>(Component::Item))
	{
		ItemComponent->OnOffItemInformation(true, Item.ItemID);
	}
}

void UItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (auto ItemComponent = static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetComponent<UItemComponent>(Component::Item))
	{
		ItemComponent->OnOffItemInformation(false);
	}
}

//FReply UItemWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	//Item->GetItem()->Use(static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerController(this, 0)->GetCharacter()));
//	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
//}

//void UItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
//{
//	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
//	//if (Item.ItemID > 0)
//	//{
//	//	WidgetCanvasPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
//	//	UDragDropOperation* DragDropOperation = NewObject<UDragDropOperation>();
//	//
//	//	DragDropOperation->DefaultDragVisual = this;
//	//	DragDropOperation->Pivot = EDragPivot::MouseDown;
//	//
//	//	OutOperation = DragDropOperation;
//	//}
//}

//void UItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
//{
//	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
//	Cast<UItemWidget>(InOperation->DefaultDragVisual)->WidgetCanvasPanel->SetVisibility(ESlateVisibility::Visible);
//}
//
//bool UItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
//{
//	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
//	UItemWidget* DragWidget = Cast<UItemWidget>(InOperation->DefaultDragVisual);
//
//	FItemData temp = DragWidget->Item;
//
//	DragWidget->Item = Item;
//	DragWidget->SetImage();
//
//	Item = temp;
//	SetImage();
//
//	return false;
//}

void UItemWidget::SetData(UItemWidget* Widget)
{
	Item = Widget->Item;
	SetImage();
}

void UItemWidget::SetImage()
{
	if (Item.ItemID > 0)
	{
		if (auto GameInstance = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			Item_Image->SetBrushFromSoftTexture(GameInstance->GetItemData(Item.ItemID)->ItemTexture.LoadSynchronous());
			Item_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (Item.ItemCount > 1) Count->SetText(FText::FromString(FString::Printf(TEXT("%d"), Item.ItemCount)));
		else Count->SetText(FText::FromString(FString("")));
	}
	else
	{
		Item_Image->SetBrushFromSoftTexture(NULL);
		Item_Image->SetVisibility(ESlateVisibility::Collapsed);
		Count->SetText(FText::FromString(FString("")));
	}
}

void UItemWidget::SetBackGroundImage(TSoftObjectPtr<UTexture2D> SoftTexture)
{
	Item_BackGroundImage->SetBrushFromSoftTexture(SoftTexture);
}

void UItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	SetData(static_cast<UItemWidget*>(ListItemObject));
}

void UItemWindowWidget::OpenItemWindowAndHolder(TArray<FItemData> Item)
{
	UpdateItem(false, Item);

	OpenItemWindow();

	ItemHolder_Border->SetVisibility(ESlateVisibility::Visible);
}

void UItemWindowWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!SelectedItem || SelectedItem->GetVisibility() != ESlateVisibility::HitTestInvisible) return;
	
	FVector2D MousePosition;
	GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y);
	MousePosition.X -= 40.;
	MousePosition.Y -= 40.;

	SelectedItem->SetPositionInViewport(MousePosition, true);
	
}

void UItemWindowWidget::OnItemClicked(UObject* MyListView)
{
	int32 ItemIndex = Item_List->GetIndexInList(MyListView);
	
	if (ItemIndex != INDEX_NONE)
	{
		if(auto ItemComponent = static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetComponent<UItemComponent>(Component::Item))
		{
			if (auto Widget = Cast<UItemWidget>(Item_List->GetItemAt(ItemIndex)))
			{
				if (SelectedItem->GetItemData().ItemID && SelectedItem->GetItemData().ItemID == Widget->GetItemData().ItemID)
				{
					if (auto Type = static_cast<uint8>(Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(SelectedItem->GetItemData().ItemID)->ItemType))
					{
						if (!Type || Type > 5)
						{
							Widget->GetItemData().ItemCount += SelectedItem->GetItemData().ItemCount;

							Cast<UItemWidget>(Item_List->GetEntryWidgetFromItem(MyListView))->SetData(Cast<UItemWidget>(MyListView));

							SelectedItem->GetItemData() = FItemData{ 0 };

							ItemComponent->ItemSelect(FItemData{ 0 });

						}
					}
				}
				else
				{
					FItemData ItemData = Widget->GetItemData();
					Widget->GetItemData() = SelectedItem->GetItemData();
					Item_List->RegenerateAllEntries();
					ItemComponent->ItemSelect(ItemData);
				}
			}

			ItemComponent->Click(true, ItemIndex);
		}
	}
	else 
		static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetComponent<UItemComponent>(Component::Item)->Click(false, ItemHolder_List->GetIndexInList(MyListView));

	/*if (auto Widget = Cast<UItemWidget>(MyListView))
	{
		if (auto TileView = Cast<UTileView>(Widget->GetOwningListView()))
		{
			if(auto ItemComponent = static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetComponent<UItemComponent>(Component::Item))
			{
				if (TileView == Item_List)
				{
					FItemData ItemData = Widget->GetItemData();
					Widget->GetItemData() = SelectedItem->GetItemData();
					Item_List->RegenerateAllEntries();
					ItemComponent->ItemSelect(ItemData);
				}

				ItemComponent->Click(TileView == Item_List, TileView->GetIndexInList(MyListView));
			}
		}
	
	}*/
}

void UItemWindowWidget::UpdateItem(bool IsPlayerItem, TArray<FItemData> Item)
{
	TArray<UObject*> NewItemData;

	for (auto& It : Item)
	{
		UItemWidget* ItemData = NewObject<UItemWidget>(this, UItemWidget::StaticClass());
		ItemData->Item = It;
		NewItemData.Emplace(ItemData);
	}

	IsPlayerItem ? Item_List->SetListItems(NewItemData) : ItemHolder_List->SetListItems(NewItemData);
}

void UItemWindowWidget::CloseItemWindow()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	SetVisibility(ESlateVisibility::Collapsed);

	static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetComponent<UItemComponent>(Component::Item)->CloseWindow();

	if (ItemHolder_Border->GetVisibility() != ESlateVisibility::Collapsed)
	{
		ItemHolder_Border->SetVisibility(ESlateVisibility::Collapsed);
		ItemHolder_List->ClearListItems();
	}
}

void UItemWindowWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Item_List->OnItemClicked().AddUFunction(this, "OnItemClicked");
	ItemHolder_List->OnItemClicked().AddUFunction(this, "OnItemClicked");
	//
	//for (int i = 0; i < 16; i++)
	//{
	//	UItemWidget* ItemData = NewObject<UItemWidget>(this, UItemWidget::StaticClass());
	//	Item_List->AddItem(ItemData);
	//}
	//
	//Equipment_Border->SetVisibility(ESlateVisibility::Collapsed);
	//
	//ConstructorHelpers::FObjectFinder<UTexture2D> FT(TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/helm.helm'"));
	//Helmet->SetBackGroundImage(FT.Object);
	//ConstructorHelpers::FObjectFinder<UTexture2D> ST(TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/armor.armor'"));
	//Armor->SetBackGroundImage(ST.Object);
	//ConstructorHelpers::FObjectFinder<UTexture2D> TT(TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/pants.pants'"));
	//Pants->SetBackGroundImage(TT.Object);
	//ConstructorHelpers::FObjectFinder<UTexture2D> FTT(TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/boots.boots'"));
	//Boots->SetBackGroundImage(FTT.Object);
	//ConstructorHelpers::FObjectFinder<UTexture2D> FFT(TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/crossed_swords.crossed_swords'"));
	//Weapon->SetBackGroundImage(FFT.Object);


	//ConstructorHelpers::FClassFinder<UUserWidget> NUW(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/Notification.Notification_C'"));
	//
	//if (NUW.Succeeded())
	//{
	//	Notification = CreateWidget(GetWorld(), NUW.Class);
	//
	//	if (Notification)
	//	{
	//
	//		Notification->AddToViewport();
	//
	//		Notification->SetVisibility(ESlateVisibility::Collapsed);
	//
	//	}
	//}
	//ConstructorHelpers::FClassFinder<UUserWidget> SI(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/PlayerItemWidgetBlueprint.PlayerItemWidgetBlueprint_C'"));
	//
	//if(SI.Succeeded())
	//{
	//	SelectedItem = CreateWidget<UItemWidget>(GetWorld(), SI.Class);
	//
	//	if(SelectedItem)
	//	{
	//		SelectedItem->AddToViewport(1);
	//
	//		SelectedItem->SetVisibility(ESlateVisibility::Collapsed);
	//	}
	//}
}

FReply UItemWindowWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (InKeyEvent.GetKey() == EKeys::E)
	{
		CloseItemWindow();

		return FReply::Handled();
	}
	//if (InKeyEvent.GetKey() == EKeys::E && OpenMode == 2)
	//{
	//	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	//	PlayerController->SetShowMouseCursor(false);
	//	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	//	SetVisibility(ESlateVisibility::Collapsed);
	//
	//	ItemBox_Border->SetVisibility(ESlateVisibility::Collapsed);
	//	ItemBox_List->ClearListItems();
	//	ItemBox->CloseBox();
	//	ItemBox = nullptr;
	//	if (Owner && Owner->GetCanGetItem())
	//	{
	//		Owner->SetNotification(ESlateVisibility::HitTestInvisible);
	//	}
	//	return FReply::Handled();
	//}
	return  FReply::Unhandled();
}


void UItemWindowWidget::OpenItemWindow()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	PlayerController->SetShowMouseCursor(true);

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, nullptr, EMouseLockMode::DoNotLock, true);
	SetKeyboardFocus();
	SetVisibility(ESlateVisibility::Visible);

	if (static_cast<ABaseCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetComponent<UEquipmentComponent>(Component::Equipment))
		Equipment_Border->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Notification->SetVisibility(ESlateVisibility::Collapsed);
}

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//ConstructorHelpers::FClassFinder<UItemWindowWidget> PUW(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/PlayerItemWindowWidgetBlueprint.PlayerItemWindowWidgetBlueprint_C'"));
	//
	//if (PUW.Succeeded())
	//{
	//
	//	UUserWidget* ItemWidget = CreateWidget(GetWorld(), PUW.Class);
	//
	//	ItemWindow = Cast<UItemWindowWidget>(ItemWidget);
	//
	//	if (ItemWindow)
	//	{
	//		ItemWindow->bIsFocusable = true;
	//
	//		ItemWindow->AddToViewport();
	//
	//		ItemWindow->SetVisibility(ESlateVisibility::Collapsed);
	//		
	//	}
	//
	//
	//}

	
	// ...
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character->IsLocallyControlled()) AddViewportWidget();
	
	if (!Character->HasAuthority()) return;
	

	for (int i = 0; i < 16; i++)
	{
		FItemData NewItem;
		NewItem.ItemID = 0;
		Item.Emplace(NewItem);
	}
}

void UItemComponent::ItemDisassembly(FItemData& ItemData)
{
	auto Num = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(ItemData.ItemID)->Disassembly;
	
	for (auto& It : Item)
	{
		if (It.ItemID == 15)
		{
			It.ItemCount += Num; 
			
			ItemData = FItemData();

			return;
		}
	}

	for (auto& It : Item)
	{
		if (It.ItemID == 0)
		{
			It.ItemID = 15;
			It.ItemCount = Num;

			ItemData = FItemData();
			return;
		}
	}

}

void UItemComponent::OpenItemWindow_Implementation()
{
	ItemWindow->OpenItemWindow();
}

void UItemComponent::UpdateItem_Implementation(bool IsPlayerItem, TArray<FItemData> const& ItemData)
{
	if (!ItemWindow) return;

	ItemWindow->UpdateItem(IsPlayerItem, ItemData);
}

void UItemComponent::UpdateEquipment_Implementation(EquipmentWidget Widget, FItemData const& ItemData)
{
	if (!ItemWindow || Widget == EquipmentWidget::None) return;

	switch (Widget)
	{
	case EquipmentWidget::Helmet:
		if (auto& Helmet = ItemWindow->Helmet)
		{
			Helmet->GetItemData() = ItemData;
			Helmet->SetImage();
		}
		break;
	case EquipmentWidget::Armor:
		if (auto& Armor = ItemWindow->Armor)
		{
			Armor->GetItemData() = ItemData;
			Armor->SetImage();
		}
		break;
	case EquipmentWidget::Pants:
		if (auto& Pants = ItemWindow->Pants)
		{
			Pants->GetItemData() = ItemData;
			Pants->SetImage();
		}
		break;
	case EquipmentWidget::Boots:
		if (auto& Boots = ItemWindow->Boots)
		{
			Boots->GetItemData() = ItemData;
			Boots->SetImage();
		}
		break;
	case EquipmentWidget::Weapon:
		if (auto& Weapon = ItemWindow->Weapon)
		{
			Weapon->GetItemData() = ItemData;
			Weapon->SetImage();
		}
		break;
	default:
		break;
	}

}

void UItemComponent::VisibleNotification_Implementation(bool IsTrue)
{
	if (!ItemWindow || !ItemWindow->GetNotification()) return;

	if (IsTrue)
		ItemWindow->GetNotification()->SetVisibility(ESlateVisibility::HitTestInvisible);
	else
		ItemWindow->GetNotification()->SetVisibility(ESlateVisibility::Collapsed);
}
void UItemComponent::OpenItemWindowAndHolder_Implementation(TArray<FItemData> const& ItemData)
{
	if (!ItemWindow) return;

	ItemWindow->OpenItemWindowAndHolder(ItemData); 
}

void UItemComponent::OpenItemHolder_Implementation()
{
	if (ItemHolders.Num() <= 0)
	{
		OpenItemWindow();

		return;
	}

	for (auto& ItemHolder : ItemHolders)
	{
		if (OpenHolder == nullptr || Character->GetDistanceTo(OpenHolder->GetActor()) > Character->GetDistanceTo(ItemHolder->GetActor()))
		{
			OpenHolder = ItemHolder;
		}
	}
	OpenHolder->Open(this);
}

void UItemComponent::CheckItemOwnership_Implementation()
{
	for (auto& It : Item)
	{
		if (It.ItemID)
		{
			UpdateItem(true, Item);
			return;
		}
	}

	if (auto Equipment = Character->GetComponent<UEquipmentComponent>(Component::Equipment))
	{
		if (Equipment->GetEquipment(ItemType::Helmet).ItemID) UpdateEquipment(EquipmentWidget::Helmet, Equipment->GetEquipment(ItemType::Helmet));
		if (Equipment->GetEquipment(ItemType::Armor).ItemID) UpdateEquipment(EquipmentWidget::Armor, Equipment->GetEquipment(ItemType::Armor));
		if (Equipment->GetEquipment(ItemType::Pants).ItemID) UpdateEquipment(EquipmentWidget::Pants, Equipment->GetEquipment(ItemType::Pants));
		if (Equipment->GetEquipment(ItemType::Boots).ItemID) UpdateEquipment(EquipmentWidget::Boots, Equipment->GetEquipment(ItemType::Boots));
		if (Equipment->GetEquipment(ItemType::Weapon).ItemID) UpdateEquipment(EquipmentWidget::Weapon, Equipment->GetEquipment(ItemType::Weapon));
		
	}
}

void UItemComponent::CopyItem(TArray<FItemData> NewItem)
{
	if (!Character->HasAuthority()) return;

	Item = NewItem;
	CheckItemOwnership();
}

void UItemComponent::AddItemHolder(ItemsHolder* NewItemHolder)
{
	ItemHolders.Emplace(NewItemHolder);

	if (ItemHolders.Num() == 1)
		VisibleNotification(true);
}

void UItemComponent::RemoveItemHolder(ItemsHolder* NewItemHolder) 
{
	ItemHolders.Remove(NewItemHolder);

	if (ItemHolders.Num() == 0)
		VisibleNotification(false);
}

void UItemComponent::CloseWindow_Implementation()
{
	if (ItemHolders.Num() > 0)
		VisibleNotification(true);

	if (!OpenHolder) return;

	OpenHolder->Close(this);
	OpenHolder = nullptr;
}
//void UItemComponent::Click(UObject* MyListView)
//{
//	int32 ItemIndex = ItemWindow->Item_List->GetIndexInList(MyListView);
//
//	if (ItemIndex != INDEX_NONE)
//	{
//		if (auto Widget = Cast<UItemWidget>(ItemWindow->Item_List->GetItemAt(ItemIndex)))
//		{
//			FItemData ItemData = Widget->GetItemData();
//			Widget->GetItemData() = ItemWindow->SelectedItem->GetItemData();
//			ItemWindow->Item_List->RegenerateAllEntries();
//			ItemSelect(ItemData);
//		}
//
//		Click(true, ItemIndex);
//	}
//	else 
//		Click(false, ItemWindow->ItemHolder_List->GetIndexInList(MyListView));
//}

void UItemComponent::Click_Implementation(bool IsPlayerItem, int32 ItemIndex)
{
	FItemData ItemData = SelectedItem;

	if (IsPlayerItem)
	{
		if (SelectedItem.ItemID == 0 && Item[ItemIndex].ItemID == 0) return;

		if (SelectedItem.ItemID == Item[ItemIndex].ItemID)
		{
			if (auto Type = static_cast<uint8>(Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(SelectedItem.ItemID)->ItemType))
			{
				if (!Type || Type > 5)
				{
					Item[ItemIndex].ItemCount += SelectedItem.ItemCount;
					SelectedItem = FItemData{ 0 };

					return;
				}
			}
		}

		SelectedItem = Item[ItemIndex];
		Item[ItemIndex] = ItemData;
		
	}
	else
	{
		if (SelectedItem.ItemID == 0 && OpenHolder->GetItem()[ItemIndex].ItemID == 0) return;

		if (SelectedItem.ItemID == OpenHolder->GetItem()[ItemIndex].ItemID)
		{
			if (auto Type = static_cast<uint8>(Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(SelectedItem.ItemID)->ItemType))
			{
				if (!Type || Type > 5)
				{
					OpenHolder->GetItem()[ItemIndex].ItemCount += SelectedItem.ItemCount;
					SelectedItem = FItemData{ 0 };

					for (auto& ConnectPlayer : OpenHolder->GetConnectPlayer())
					{
						ConnectPlayer->UpdateItem(false, OpenHolder->GetItem());
					}

					ItemSelect(FItemData{ 0 });

					return;
				}
			}
		}

		SelectedItem = OpenHolder->GetItem()[ItemIndex];
		OpenHolder->GetItem()[ItemIndex] = ItemData;
		for (auto& ConnectPlayer : OpenHolder->GetConnectPlayer())
		{
			ConnectPlayer->UpdateItem(false, OpenHolder->GetItem());
		}

		ItemSelect(SelectedItem);
	}
	
}

void UItemComponent::Call_Implementation(int IsTrue)
{
	UE_LOG(LogTemp, Log, TEXT("Call %d"), IsTrue);
}

void UItemComponent::Use_Implementation(bool IsPlayerItem, int32 ItemIndex)
{
	FItemData* Data; 

	if (IsPlayerItem && Item[ItemIndex].ItemID != 0)	Data = &Item[ItemIndex];
	else if (!IsPlayerItem && OpenHolder->GetItem()[ItemIndex].ItemID != 0) Data = &OpenHolder->GetItem()[ItemIndex];
	else return;

	if (auto GameInstance = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (GameInstance->GetItemData(Data->ItemID)->ItemType == ItemType::None)
		{

		}
		else if (static_cast<uint8>(GameInstance->GetItemData(Data->ItemID)->ItemType) < 6)
		{
			if (Character->IsA(ATank::StaticClass()) || Character->IsA(ASoldier::StaticClass()))
			{
				ItemDisassembly(*Data);

				if (!IsPlayerItem) UpdateItem(true, Item);
			}
			else if (auto Equipment = Character->GetComponent<UEquipmentComponent>(Component::Equipment))
			{
				Equipment->SetEquipment(*Data);
			}
		}
		else if (GameInstance->GetItemData(Data->ItemID)->ItemType == ItemType::Potion && !Character->IsA(ATank::StaticClass()))
			Potion::Use(Character, *Data);
		else if (GameInstance->GetItemData(Data->ItemID)->ItemType == ItemType::Iron && Character->IsA(ATank::StaticClass()))
			Potion::Use(Character, *Data);
		
	}

	if (IsPlayerItem)
	{
		UpdateItem(true, Item);
	}
	else
	{
		for (auto& ConnectPlayer : OpenHolder->GetConnectPlayer())
		{
			ConnectPlayer->UpdateItem(false, OpenHolder->GetItem());
		}
	}
}

void UItemComponent::ItemSelect_Implementation(FItemData ItemData)
{
	if (!ItemWindow) return;

	ItemData.ItemID != 0 ? ItemWindow->SelectedItem->SetVisibility(ESlateVisibility::HitTestInvisible) : ItemWindow->SelectedItem->SetVisibility(ESlateVisibility::Collapsed);
	
	ItemWindow->SelectedItem->GetItemData() = ItemData;
	ItemWindow->SelectedItem->SetImage();

}

void UItemComponent::UnwearEquipment_Implementation(EquipmentWidget Widget)
{
	for (auto& It : Item)
	{
		if (It.ItemID == 0)
		{
			if (auto Equipment = Character->GetComponent<UEquipmentComponent>(Component::Equipment))
			{
				Equipment->UnwearEquipment(Widget, It);

				if(It.ItemID)
				{
					UpdateEquipment(Widget, FItemData{ 0 });

					UpdateItem(true, Item);
				}

			}

			return;
		}
	}

}

void UItemComponent::DestroyWidget()
{
	if (ItemWindow->SelectedItem)
	{
		ItemWindow->SelectedItem->RemoveFromParent();  // ������ �θ𿡼� ����
		ItemWindow->SelectedItem = nullptr;             // ���۷��� �ʱ�ȭ
	}
	if (ItemWindow->Notification)
	{
		ItemWindow->Notification->RemoveFromParent();  // ������ �θ𿡼� ����
		ItemWindow->Notification = nullptr;             // ���۷��� �ʱ�ȭ
	}
	if (ItemWindow->PotionUseWidget)
	{
		ItemWindow->PotionUseWidget->RemoveFromParent();  // ������ �θ𿡼� ����
		ItemWindow->PotionUseWidget = nullptr;             // ���۷��� �ʱ�ȭ
	}
	if (ItemWindow)
	{
		ItemWindow->RemoveFromParent();  // ������ �θ𿡼� ����
		ItemWindow = nullptr;             // ���۷��� �ʱ�ȭ
	}
}

void UItemComponent::AddViewportWidget()
{
	if (ItemWindow) return;

	if (auto PUW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/PlayerItemWindowWidgetBlueprint.PlayerItemWindowWidgetBlueprint_C'")))
	{
	
		ItemWindow = CreateWidget<UItemWindowWidget>(GetWorld(), PUW);
	
		if (ItemWindow)
		{
			ItemWindow->SetIsFocusable(true);
	
			ItemWindow->AddToViewport();
	
			ItemWindow->SetVisibility(ESlateVisibility::Collapsed);

			for (int i = 0; i < 16; i++)
			{
				ItemWindow->Item_List->AddItem(NewObject<UItemWidget>(this, UItemWidget::StaticClass()));
			}

			CheckItemOwnership();

			ItemWindow->Equipment_Border->SetVisibility(ESlateVisibility::Collapsed);

			if (auto FT = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/helm.helm'")))
			{
				ItemWindow->Helmet->SetBackGroundImage(FT);
			}
			if (auto ST = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/armor.armor'")))
			{
				ItemWindow->Armor->SetBackGroundImage(ST);
			}
			if (auto TT = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/pants.pants'")))
			{
				ItemWindow->Pants->SetBackGroundImage(TT);
			}
			if (auto FTT = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/boots.boots'")))
			{
				ItemWindow->Boots->SetBackGroundImage(FTT);
			}
			if (auto FFT = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/300+FlatIcons/inventory_and_statuses/crossed_swords.crossed_swords'")))
			{
				ItemWindow->Weapon->SetBackGroundImage(FFT);
			}

			if (auto SI = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/PlayerItemWidgetBlueprint.PlayerItemWidgetBlueprint_C'")))
			{
				ItemWindow->SelectedItem = CreateWidget<UItemWidget>(GetWorld(), SI);

				if (ItemWindow->SelectedItem)
				{
					ItemWindow->SelectedItem->AddToViewport(1);

					ItemWindow->SelectedItem->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

			if (auto NUW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/Notification.Notification_C'")))
			{
				ItemWindow->Notification = CreateWidget(GetWorld(), NUW);

				if (ItemWindow->Notification)
				{

					ItemWindow->Notification->AddToViewport();

					ItemWindow->Notification->SetVisibility(ESlateVisibility::Collapsed);

				}
			}
			
			if (auto IIW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/ItemInformationWidgetBlueprint.ItemInformationWidgetBlueprint_C'")))
			{
				ItemWindow->Information = CreateWidget<UItemInformationWidget>(GetWorld(), IIW);

				if (ItemWindow->Information)
				{

					ItemWindow->Information->AddToViewport();

					ItemWindow->Information->SetVisibility(ESlateVisibility::Collapsed);

				}
			}
			
			if (auto PW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/PotionUseWidgetBlueprint.PotionUseWidgetBlueprint_C'")))
			{
				ItemWindow->PotionUseWidget = CreateWidget<UPotionUseWidget>(GetWorld(), PW);

				if (ItemWindow->PotionUseWidget)
				{

					ItemWindow->PotionUseWidget->AddToViewport();

					ItemWindow->PotionUseWidget->SetVisibility(ESlateVisibility::Collapsed);

				}
			}
		}
	
	}
	
}

void UItemComponent::OnOffItemInformation(bool IsTrue, uint8 ItemID)
{
	if (!ItemWindow || !ItemWindow->Information) return;
	
	if (IsTrue && ItemID)
	{
		ItemWindow->Information->SetData(ItemID);

		FVector2D MousePosition;
		GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y);
		ItemWindow->Information->SetPositionInViewport(MousePosition, true);

		ItemWindow->Information->SetVisibility(ESlateVisibility::HitTestInvisible);

	}
	else
	{
		ItemWindow->Information->SetVisibility(ESlateVisibility::Collapsed);
	}
}

EquipmentWidget UItemComponent::CheckEquipmentWidget(UItemWidget* Widget)
{
	if (!ItemWindow) return EquipmentWidget::None;

	if (ItemWindow->Helmet == Widget) return EquipmentWidget::Helmet;
	if (ItemWindow->Armor == Widget) return EquipmentWidget::Armor;
	if (ItemWindow->Pants == Widget) return EquipmentWidget::Pants;
	if (ItemWindow->Boots == Widget) return EquipmentWidget::Boots;
	if (ItemWindow->Weapon == Widget) return EquipmentWidget::Weapon;
	return EquipmentWidget::None;
}

bool UItemComponent::CheckItemTypeCount(ItemType Type, uint8 RequiredCount)
{
	int ItemCount = 0;

	for (auto& It : Item)
	{
		if (It.ItemID == 0) continue;

		if (Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(It.ItemID)->ItemType == Type) ItemCount += It.ItemCount;
		
	}

	return ItemCount >= RequiredCount ? true : false;
	
}

void UItemComponent::RemoveItemTypeCount(ItemType Type, uint8 RequiredCount)
{
	if (Character->GetLocalRole() != ROLE_Authority) return;

	for (auto& It : Item)
	{
		if (It.ItemID == 0 || Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(It.ItemID)->ItemType != Type) continue;

		if (It.ItemCount <= RequiredCount)
		{
			RequiredCount -= It.ItemCount;
			It = FItemData();

			if (RequiredCount == 0) break;
		}
		else
		{
			It.ItemCount -= RequiredCount;
			
			break;
		}
	}

	UpdateItem(true, Item);
}

void UItemComponent::StartUsePotion(FItemData& ItemData)
{
	if (ItemData.ItemID == 0) return;

	auto HealingTime = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(ItemData.ItemID)->HealingTime;

	GetWorld()->GetTimerManager().SetTimer(UsePotionHandle, [&]()
		{
			SuccessUsePotion(ItemData);
		}, HealingTime, false);

	StartPotionUseProgress(HealingTime);
}

void UItemComponent::SuccessUsePotion(FItemData& ItemData)
{
	if (auto Health = Character->GetComponent<UHealthComponent>(Component::Health))
	{
		Health->SetCurrentHealth(Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(ItemData.ItemID)->Health + Health->GetCurrentHealth());

		if (ItemData.ItemCount == 1) ItemData.ItemID = 0;
		else ItemData.ItemCount -= 1;

		StopPotionUseProgress();
	}

}

void UItemComponent::StartPotionUseProgress_Implementation(double Time)
{
	if (ItemWindow && ItemWindow->PotionUseWidget)
	{
		ItemWindow->CloseItemWindow();

		ItemWindow->PotionUseWidget->SetTime(Time);

		GetWorld()->GetTimerManager().SetTimer(UsePotionHandle, this, &UItemComponent::SetPotionUseProgress, 0.05f, true);
	}
}

void UItemComponent::StopPotionUseProgress_Implementation()
{
	if (ItemWindow && ItemWindow->PotionUseWidget)
	{
		ItemWindow->PotionUseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	GetWorld()->GetTimerManager().ClearTimer(UsePotionHandle);
}

void UItemComponent::SetPotionUseProgress()
{
	if (ItemWindow && ItemWindow->PotionUseWidget)
	{
		auto Remaining = ItemWindow->PotionUseWidget->SetPercent(GetWorld()->GetTimerManager().GetTimerElapsed(UsePotionHandle));

		if (Remaining <= 0)
		{
			ItemWindow->PotionUseWidget->SetVisibility(ESlateVisibility::Collapsed);

			GetWorld()->GetTimerManager().ClearTimer(UsePotionHandle);
		}

	}
}

bool UItemComponent::IsUsingPotion()
{
	return GetWorld()->GetTimerManager().IsTimerActive(UsePotionHandle);
}

void UItemComponent::StopUsingPotion_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(UsePotionHandle);
}

void UEquipmentComponent::SetEquipment(FItemData& Item)
{
	if (Item.ItemID == 0) return;

	FItemData UnwearEquipment = { 0 };

	if (auto GameInstance = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (GameInstance->GetItemData(Item.ItemID)->ItemType != ItemType::None && static_cast<uint8>(GameInstance->GetItemData(Item.ItemID)->ItemType) > 5) return;
		Character->GetComponent<UItemComponent>(Component::Item)->UpdateEquipment(
			static_cast<EquipmentWidget>(static_cast<std::underlying_type_t<EquipmentWidget>>(GameInstance->GetItemData(Item.ItemID)->ItemType)), Item);

		switch (GameInstance->GetItemData(Item.ItemID)->ItemType)
		{
		case ItemType::Helmet:
			if (Helmet.ItemID)
				UnwearEquipment = Helmet;
			Helmet = Item;
			break;
		case ItemType::Armor:
			if (Armor.ItemID)
				UnwearEquipment = Armor;
			Armor = Item;
			break;
		case ItemType::Pants:
			if (Pants.ItemID)
				UnwearEquipment = Pants;
			Pants = Item;
			break;
		case ItemType::Boots:
			if (Boots.ItemID)
				UnwearEquipment = Boots;
			Boots = Item;
			break;
		case ItemType::Weapon:
			if (Weapon.ItemID)
				UnwearEquipment = Weapon;
			Weapon = Item;
			break;
		default:
			break;
		}

	}

	Item = UnwearEquipment;


}

FItemData UEquipmentComponent::GetEquipment(ItemType ItemType)
{
	switch (ItemType)
	{
	case ItemType::Helmet:
		return Helmet;
		break;
	case ItemType::Armor:
		return Armor;
		break;
	case ItemType::Pants:
		return Pants;
		break;
	case ItemType::Boots:
		return Boots;
		break;
	case ItemType::Weapon:
		return Weapon;
		break;
	default:
		return FItemData();
		break;
	}
}

void UEquipmentComponent::UnwearEquipment(EquipmentWidget Widget, FItemData& Item)
{
	FItemData UnwearEquipment = { 0 };

	switch (Widget)
	{
	case EquipmentWidget::Helmet:
		UnwearEquipment = Helmet;
		Helmet = Item;
		break;
	case EquipmentWidget::Armor:
		UnwearEquipment = Armor;
		Armor = Item;
		break;
	case EquipmentWidget::Pants:
		UnwearEquipment = Pants;
		Pants = Item;
		break;
	case EquipmentWidget::Boots:
		UnwearEquipment = Boots;
		Boots = Item;
		break;
	case EquipmentWidget::Weapon:
		UnwearEquipment = Weapon;
		Weapon = Item;
		break;
	default:
		break;
	}

	Item = UnwearEquipment;
}

void UPotionUseWidget::SetTime(double NewTime)
{
	Time = NewTime;
	RemainingTime->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Time)));
	PB_RemainingTimeBar->SetPercent(0);

	SetVisibility(ESlateVisibility::HitTestInvisible);
}

double UPotionUseWidget::SetPercent(double NewTime)
{
	auto Remaining = FCString::Atod(*RemainingTime->GetText().ToString()) - NewTime;

	PB_RemainingTimeBar->SetPercent((Time - Remaining) / Time);
	RemainingTime->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Remaining)));

	return Remaining;
}
