// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class YUWIBO_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
protected:
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY()
	class UProjectilePool* ProjectilePool;

	virtual void Disappear();
public:	
	FORCEINLINE void SetProjectilePool(UProjectilePool* NewProjectilePool) { ProjectilePool = NewProjectilePool; };

	virtual void Activate();

	UFUNCTION(NetMulticast, Reliable)
	void OnActivate();

	void Deactivate();

	UFUNCTION(NetMulticast, Reliable)
	void OnDeactivate();
};
