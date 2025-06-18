// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Bullet.generated.h"

UCLASS()
class YUWIBO_API ABullet : public AProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

private:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* StaticMesh;

private:
	UFUNCTION()
	void OverlapBullet(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void HitBullet(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
