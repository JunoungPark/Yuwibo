// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "BodyChanger.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ABodyChanger : public AMoveCharacter
{
	GENERATED_BODY()

	ABodyChanger();
private:
	UPROPERTY(VisibleAnywhere)
	class UProjectilePool* ProjectilePool;
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* TeleportParticleComponent;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* SecondTeleportParticleComponent;
	UPROPERTY(VisibleAnywhere)
		class UAudioComponent* MagicAudio;
	UPROPERTY(VisibleAnywhere)
		class UAudioComponent* TeleportAudio;
public:
	void BeginPlay() override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(Server, Reliable)
	void GetWaterBall(FVector Location, FRotator Rotation);
	UFUNCTION(Server, Reliable)
	void Teleportation();
	void PlayMagicSound();
	UFUNCTION(Client, Reliable)
	void PlayTeleportSound();
	UFUNCTION(Server, Reliable)
	void BodyChange();

	UFUNCTION(NetMulticast, Reliable)
	void Active();

	void Tick(float DeltaTime) override;
};
