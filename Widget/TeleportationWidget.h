// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeleportationWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UTeleportationWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	FVector2D Location;

	FTimerHandle Timer;

	uint8 Count = 10;

	int8 ButtonIndex = -1;

	UPROPERTY(meta = (BindWidget))
	class UImage* Map;

	UPROPERTY(meta = (BindWidget))
	class UButton* FirstSpawnLocation;

	UPROPERTY(meta = (BindWidget))
	class UButton* SecondSpawnLocation;

	UPROPERTY(meta = (BindWidget))
	class UButton* ThirdSpawnLocation;

	UPROPERTY(meta = (BindWidget))
	class UButton* FourthSpawnLocation;

	UPROPERTY(meta = (BindWidget))
	class UButton* FifthSpawnLocation;

	UPROPERTY(meta = (BindWidget))
	class UButton* SixthSpawnLocation;

	UPROPERTY(meta = (BindWidget))
	class UButton* SeventhSpawnLocation;

	UPROPERTY(meta = (BindWidget))
	class UButton* EighthSpawnLocation;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RemainingTime;
private:
	void SetText();

	UFUNCTION()
	void SetLocation();

public:
	void NativeOnInitialized()override;

	FString GetText();
	
	uint8 GetLocation();
};
