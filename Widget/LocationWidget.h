// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LocationWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ULocationWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class UMapWidget;
private:
	AActor* Actor;
	FVector2D Location;
	UPROPERTY(meta = (BindWidget))
	class UImage* MapLocation;
	UPROPERTY(meta = (BindWidget))
	class UImage* MiniMapLocation; 
	
public:
	void SetMapLocation(FVector2D Vector, double Size = 1.);

	void SetActor(AActor* NewActor);

	void SetMiniMapMode(bool IsTrue);

	FORCEINLINE AActor* GetActor() { return Actor; }
};
