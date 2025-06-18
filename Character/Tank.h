// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "Yuwibo/GunDefaultCameraShakeBase.h"
#include "Tank.generated.h"

UCLASS()
class YUWIBO_API ATank : public AMoveCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();


private:
	UPROPERTY(VisibleAnywhere)
	class UProjectilePool* ProjectilePool;
	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* LeftMuzzleFx;
	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* RightMuzzleFx;
	UPROPERTY(VisibleAnywhere)
		class UNiagaraSystem* MuzzleFx;
	UPROPERTY(VisibleAnywhere)
		class UNiagaraSystem* OverHeatFx;
	UPROPERTY(VisibleAnywhere)
		bool isHeatLeftMuzzle;
	UPROPERTY(VisibleAnywhere)
		bool isHeatRightMuzzle;
	UPROPERTY(VisibleAnywhere)
		class UAudioComponent* GunAudio;
	UPROPERTY(VisibleAnywhere)
		USoundWave* SpiderSound;
	UPROPERTY(VisibleAnywhere)
		USoundWave* StayRollSound;
	UPROPERTY(VisibleAnywhere)
		USoundWave* MoveRollSound;
	UPROPERTY(VisibleAnywhere)
		AHUD* GunHUD;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UGunDefaultCameraShakeBase> CameraShake = UGunDefaultCameraShakeBase::StaticClass();

		UINT8 IsShake;
	UPROPERTY(VisibleAnywhere)
	bool Upgrading = false;
	UPROPERTY(VisibleAnywhere)
	ATank* NextBody = nullptr;
protected:
	UPROPERTY(VisibleAnywhere)
	bool TankMode = false;
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* SecondStaticMesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ThirdStaticMesh;
private:
	UFUNCTION(Server, Reliable)
	void Upgrade();


	UFUNCTION(Server, Reliable)
	void UpgradePossess();

	UFUNCTION(NetMulticast, Reliable)
	void Visible(bool IsTrue);

public:
	void BeginPlay() override;
	
	class AProjectile* GetBullet(FVector Location, FRotator Rotation);
	void ResetLeftFx();
	void ResetRightFx();
	void PlayGunSound();
	int GetRollSound();
	void SetRollSound();


	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)override;
	void Tick(float DeltaTime)override;

	UFUNCTION()
	void OnOffAttack(bool IsTrue);

	void OnOffShake(bool IsTrue);

	virtual void Jump()override;

	UFUNCTION(Server, Reliable)
	void ModeChange();

	UFUNCTION(NetMulticast, Reliable)
	void ChangeMode();

	UFUNCTION(NetMulticast, Reliable)
	void MoveUpdate(FVector vector, FRotator rotation);

	UFUNCTION(BlueprintPure)
		FORCEINLINE UAudioComponent* GetFootAudio() const { return FootAudio; };

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool GetMode() const { return TankMode; };

	FORCEINLINE virtual FVector GetCenterLocation() { return SkeletalMesh->GetComponentLocation(); };

	FORCEINLINE FRotator GetLookGun() { return StaticMesh->GetComponentRotation(); };

	FORCEINLINE void SetLookGun(double LookYawAngle, double LookPitchAngle) { StaticMesh->SetWorldRotation(FRotator(0.f, LookYawAngle, LookPitchAngle)); };

	UFUNCTION(NetMulticast, Reliable)
	void Alram(float Delta);

	UFUNCTION(Client, Reliable)
	void Disable(bool IsTrue);
};
