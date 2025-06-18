// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Online/Auth.h"
#include "Online/OnlineServices.h"
#include "YuwiboGameInstance.generated.h"

UENUM()
enum class ItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Helmet UMETA(DisplayName = "헬멧"),
	Armor UMETA(DisplayName = "갑옷"),
	Pants UMETA(DisplayName = "바지"),
	Boots UMETA(DisplayName = "부츠"),
	Weapon UMETA(DisplayName = "무기"),
	Potion UMETA(DisplayName = "물약"),
	Iron UMETA(DisplayName = "철")
};
/**
 * 
 */
USTRUCT()
struct FYuwiboItemData : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> ItemTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Health; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HealingTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Disassembly;
};

USTRUCT()
struct FYuwiboCharacterData : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool RotationToMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UTexture2D>> SkillTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Information;
};

UCLASS()
class YUWIBO_API UYuwiboGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UYuwiboGameInstance();
private:
	UFUNCTION()
	void LoginClient();

	//UFUNCTION(Server, Reliable)
	//void LoginServer(TOnlineId<OnlineIdHandleTags::FAccount> AccountId);
public:
	virtual void Init() override;

	virtual void Shutdown()override;

public:
	FYuwiboCharacterData* GetCharacterData(FName CharacterName);
	FYuwiboItemData* GetItemData(uint8 ItemID);

	FORCEINLINE UE::Online::FAccountId GetAccountID() { return AccountID; }

private:
		class UDataTable* CharacterDataTable;
		class UDataTable* ItemDataTable;

		UE::Online::FAccountId AccountID;

		FName SessionName;
};
