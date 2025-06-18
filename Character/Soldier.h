// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "Soldier.generated.h"

UCLASS()
class YUWIBO_API ASoldier : public AMoveCharacter
{
	GENERATED_BODY()
public:
	ASoldier();
private:
	UPROPERTY()
	TArray<AActor*> Drones;
	UPROPERTY()
	TArray<AActor*> Turrets;
	UPROPERTY()
	ACharacter* LookCharacter;

private:
	//UFUNCTION()
	//void OverlapCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
	//void OverlapEndCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SetTarget();

	UFUNCTION(Server, Reliable)
	void ResetTarget();

	UFUNCTION()
	void TargetOutline();
public:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void CreateDrone();

	UFUNCTION(Server, Reliable)
	void CreateTurret();

	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
