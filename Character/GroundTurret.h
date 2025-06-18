// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"	
#include "GroundTurret.generated.h"

UCLASS()
class YUWIBO_API AGroundTurret : public AMoveCharacter
{
	GENERATED_BODY()
private:
	//void FindTarget();

	UPROPERTY(VisibleAnywhere)
	class UProjectilePool* ProjectilePool;

	//UPROPERTY(Replicated)
	//FRotator GunRotation;
public:
	// Sets default values for this pawn's properties
	AGroundTurret();
//protected:
//
//	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	void BeginPlay()override;

	class AProjectile* GetBullet(FVector Location, FRotator Rotation);
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//FRotator GetGunRotation() { return GunRotation; };

	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
