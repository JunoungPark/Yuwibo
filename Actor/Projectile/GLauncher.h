// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "GLauncher.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API AGLauncher : public AProjectile
{
	GENERATED_BODY()
	AGLauncher();

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;
protected:
	void Disappear()override;
private:

	UFUNCTION()
	void OverlapGLauncher(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HitGLauncher(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Attack();

	UFUNCTION(NetMulticast, Reliable)
	void OnOverlapOrHit();

	UFUNCTION(NetMulticast, Reliable)
	void OnOverlapOrHitEnd();
public:

	void BeginPlay()override;

	void Activate()override;

};
