// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInformationWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UItemInformationWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FirstInformation;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SecondInformation;
	
public:
	void SetData(uint8 ItemID);
};
