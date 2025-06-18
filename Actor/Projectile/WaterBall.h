// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "WaterBall.generated.h"

UCLASS()
class YUWIBO_API AWaterBall : public AProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterBall();
private:
	UPROPERTY(EditAnywhere)
		class USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* ParticleComponent; 
	UPROPERTY(EditAnywhere)
		class UParticleSystem* DefalutParticleSystem;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* ParticleSystem;

	void Disappear()override;

	void Attack();

	UFUNCTION()
	void OverlapWaterBall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HitWaterBall(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void OnOverlapOrHit();

	UFUNCTION(NetMulticast, Reliable)
	void OnOverlapOrHitEnd();
public:

	void Activate()override;

	UFUNCTION(NetMulticast, Reliable)
	void On();

};
