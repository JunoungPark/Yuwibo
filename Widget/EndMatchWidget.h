// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndMatchWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UEndMatchWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RankText;
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* EndMatchAnimation;
protected:

	void NativeOnInitialized()override;

	void NativeConstruct()override;

	UFUNCTION()
	void Exit();
public:
	void SetRank(uint8 MaxPlayer, uint8 Rank);
};
