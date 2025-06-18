// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "Golem.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API AGolem : public AMoveCharacter
{
	GENERATED_BODY()
public:
	AGolem();
private:
	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Niagara;
	UPROPERTY(VisibleAnywhere)
		float FlightHeight;
public:
	void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Fly(float DeltaSeconds);
	virtual void Jump()override;
	virtual void Fall();
};
