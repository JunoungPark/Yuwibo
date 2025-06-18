// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "Vampire.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API AVampire : public AMoveCharacter
{
	GENERATED_BODY()
	AVampire();
private:
	UFUNCTION()
	void HitBody(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	//void LookLeftRight(float value)override;
public:

	void BeginPlay() override;
};
