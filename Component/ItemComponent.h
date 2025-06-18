// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/Border.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"

UENUM()
enum class EquipmentWidget : uint8
{
	None,
	Helmet,
	Armor,
	Pants,
	Boots,
	Weapon,

};

USTRUCT()
struct YUWIBO_API FItemData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	uint8 ItemID = 0;

	UPROPERTY()
	uint8 ItemCount = 1;
};

class Potion
{
public:
	static void Use(ABaseCharacter* Character, FItemData& ItemData);
};



class YUWIBO_API ItemsHolder
{
protected:
	TArray<FItemData> Item;

public:
	TArray<FItemData>& GetItem() { return Item; };

	virtual void Open(UItemComponent* ItemComponent) = 0;

	virtual void Close(UItemComponent* ItemComponent) = 0;

	virtual TSet<UItemComponent*>  GetConnectPlayer() = 0;

	virtual AActor* GetActor() = 0;
};

UCLASS()
class YUWIBO_API UPotionUseWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_RemainingTimeBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RemainingTime;

	double Time;
public:
	void SetTime(double NewTime);

	double SetPercent(double NewTime);

};

UCLASS()
class YUWIBO_API UItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	friend class UItemWindowWidget;
private:
	FItemData Item;
	UPROPERTY(meta = (BindWidget))
	class UImage* Item_BackGroundImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* Item_Image;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Count;
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* WidgetCanvasPanel;
private:
	void SetData(class UItemWidget* Widget);

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override; 
	
	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override; 
	
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent)override;
protected:
	
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
public:
	void SetImage();

	void SetBackGroundImage(TSoftObjectPtr<UTexture2D> SoftTexture);

	FORCEINLINE FItemData& GetItemData() { return Item; }
	 
	UFUNCTION(BlueprintPure)
	FORCEINLINE class UCanvasPanel* GetWidgetCanvasPanel() const { return WidgetCanvasPanel; }

};

UCLASS()
class YUWIBO_API UItemWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class UItemComponent;
private:

	UPROPERTY(meta = (BindWidget))
	class UTileView* Item_List;
	UPROPERTY(meta = (BindWidget))
	class UBorder* ItemHolder_Border;
	UPROPERTY(meta = (BindWidget))
	class UTileView* ItemHolder_List;
	UPROPERTY(meta = (BindWidget))
	class UBorder* Equipment_Border;

	UPROPERTY(meta = (BindWidget))
	UItemWidget*  Helmet;
	UPROPERTY(meta = (BindWidget))
	UItemWidget*  Armor;
	UPROPERTY(meta = (BindWidget))
	UItemWidget*  Pants;
	UPROPERTY(meta = (BindWidget))
	UItemWidget*  Boots;
	UPROPERTY(meta = (BindWidget))
	UItemWidget*  Weapon;
	UPROPERTY()
	UItemWidget* SelectedItem;
	UPROPERTY()
	class UItemInformationWidget* Information;
	UPROPERTY()
	UUserWidget* Notification;
	UPROPERTY()
	UPotionUseWidget* PotionUseWidget;
private:

	void OpenItemWindowAndHolder(TArray<FItemData> Item);

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnItemClicked(UObject* MyListView);

	void UpdateItem(bool IsPlayerItem, TArray<FItemData> Item);

	void CloseItemWindow();
protected:

	virtual void NativeOnInitialized();

	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:

	FORCEINLINE UUserWidget* GetNotification() { return Notification; };

	void OpenItemWindow();
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UItemComponent : public UActorComponent, public ItemsHolder
{
	GENERATED_BODY()
private:
	TArray<ItemsHolder*> ItemHolders;

	ItemsHolder* OpenHolder = nullptr;

	UPROPERTY()
	UItemWindowWidget* ItemWindow = nullptr;

	FItemData SelectedItem = { 0 };

	FTimerHandle UsePotionHandle; 

	UPROPERTY()
	ABaseCharacter* Character = nullptr;
public:
	// Sets default values for this component's properties
	UItemComponent();
private:
	void BeginPlay()override;

	UFUNCTION(Client, Reliable)
	void OpenItemWindow();

	UFUNCTION(Client, Reliable)
	void UpdateItem(bool IsPlayerItem, TArray<FItemData> const& ItemData);

	UFUNCTION(Client, Reliable)
	void VisibleNotification(bool IsTrue);

	UFUNCTION(Client, Reliable)
	void Call(int IsTrue);

	void ItemDisassembly(FItemData& ItemData);

public:
	UFUNCTION(Client, Reliable)
	void OpenItemWindowAndHolder(TArray<FItemData> const& ItemData);

	UFUNCTION(Client, Reliable)
	void UpdateEquipment(EquipmentWidget Widget, FItemData const& ItemData);

	UFUNCTION(Server, Reliable)
	void OpenItemHolder();

	UFUNCTION(Server, Reliable)
	void CheckItemOwnership();

	void CopyItem(TArray<FItemData> NewItem);

	void AddItemHolder(ItemsHolder* NewItemHolder);

	void RemoveItemHolder(ItemsHolder* NewItemHolder);

	UFUNCTION(Server, Reliable)
	void CloseWindow();

	UFUNCTION(Server, Reliable)
	void Click(bool IsPlayerItem, int32 ItemIndex);

	UFUNCTION(Client, Reliable)
	void ItemSelect(FItemData ItemData);

	UFUNCTION(Server, Reliable)
	void Use(bool IsPlayerItem, int32 ItemIndex);

	void DestroyWidget();

	void AddViewportWidget();

	void OnOffItemInformation(bool IsTrue, uint8 ItemID = 0);

	UFUNCTION(Server, Reliable)
	void UnwearEquipment(EquipmentWidget WidgetNumber);

	EquipmentWidget CheckEquipmentWidget(UItemWidget* Widget);

	bool CheckItemTypeCount(ItemType Type, uint8 RequiredCount);

	void RemoveItemTypeCount(ItemType Type, uint8 RequiredCount);

	void StartUsePotion(FItemData& ItemData);

	void SuccessUsePotion(FItemData& ItemData);

	UFUNCTION(Client, Reliable)
	void StartPotionUseProgress(double Time);

	UFUNCTION(Client, Reliable)
	void StopPotionUseProgress();

	void SetPotionUseProgress();

	bool IsUsingPotion();

	UFUNCTION(Server, Reliable)
	void StopUsingPotion();

	FORCEINLINE bool CheckPlayerItem(UTileView* TileView) { return TileView == ItemWindow->Item_List; };

	FORCEINLINE void Bind(ABaseCharacter* NewCharacter) { Character = NewCharacter; }

	void Open(UItemComponent* ItemComponent) {};

	void Close(UItemComponent* ItemComponent) {};

	TSet<UItemComponent*>  GetConnectPlayer() { return TSet<UItemComponent*>(); };

	AActor* GetActor() { return nullptr; };


};

UCLASS()
class YUWIBO_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY()
	ABaseCharacter* Character = nullptr;

	FItemData Helmet = { 0 };
	FItemData Armor = { 0 };
	FItemData Pants = { 0 };
	FItemData Boots = { 0 };
	FItemData Weapon = { 0 };

public:
	// Called every frame
	void SetEquipment(FItemData& Item);

	FItemData GetEquipment(ItemType ItemType);

	void UnwearEquipment(EquipmentWidget WidgetNumber, FItemData& Item);

	FORCEINLINE void Bind(ABaseCharacter* NewCharacter) { Character = NewCharacter; }
};