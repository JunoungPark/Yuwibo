// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PointWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UPointWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UImage* FirstPoint;
	UPROPERTY(meta = (BindWidget))
	class UImage* SecondPoint;
	UPROPERTY(meta = (BindWidget))
	class UImage* ThirdPoint;
	
public:
	UFUNCTION(Client, Reliable)
	void AddPoint();
};
