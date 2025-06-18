// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "RockMonster.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ARockMonster : public AMoveCharacter
{
	GENERATED_BODY()

		ARockMonster();

public:
	void BeginPlay() override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//void LookLeftRight(float value)override;
};
