// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UDragUserWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
		class UImage* Item_Image;
public:

	UFUNCTION(BlueprintPure)
	class UImage* GetItemImage() const;
};
