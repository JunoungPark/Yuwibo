// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "TankLv2.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ATankLv2 : public ATank
{
	GENERATED_BODY()
public:
	ATankLv2();
protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SecondSkeletalMesh;

public:
	void BeginPlay() override;

	FORCEINLINE FVector GetCenterLocation() override { return StaticMesh->GetComponentLocation(); };
};
