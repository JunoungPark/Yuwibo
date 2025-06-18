// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "CrustaspikanLarvae.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ACrustaspikanLarvae : public AMoveCharacter
{
	GENERATED_BODY()
		ACrustaspikanLarvae();
	//void LookLeftRight(float value)override;
private:
	UFUNCTION()
	void HitBody(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:

	void BeginPlay() override;
};
