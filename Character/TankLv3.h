// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankLv2.h"
#include "Yuwibo/Animinstance/TankBackGunAnimInstance.h"
#include "TankLv3.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ATankLv3 : public ATankLv2
{
	GENERATED_BODY()
public:
	ATankLv3();
private:
	UPROPERTY(VisibleAnywhere)
	class UProjectilePool* GLauncherProjectilePool;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ThirdSkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BackSkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FourthStaticMesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FourthSkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FifthSkeletalMesh;

	UPROPERTY(ReplicatedUsing = OnRep_Camera)
	ACameraActor* Camera = nullptr;

private:
	UFUNCTION(Server, Reliable)
	void WeaponChange();

	UFUNCTION(NetMulticast, Reliable)
	void ChangeWeapon(bool IsGLauncherMode);

	UFUNCTION()
	void OnRep_Camera();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	void BeginPlay() override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)override;

	AProjectile* GetGLauncher(FVector Location, FRotator Rotation);
	
	FORCEINLINE ACameraActor* GetCamera() { return Camera; }

	FORCEINLINE UTankBackGunAnimInstance* GetBackSleletalAnim() { return static_cast<UTankBackGunAnimInstance*>(BackSkeletalMesh->GetAnimInstance()); };

	FORCEINLINE FVector GetCenterLocation() override { return ThirdSkeletalMesh->GetComponentLocation(); };
};
