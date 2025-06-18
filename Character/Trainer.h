// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "Trainer.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ATrainer : public AMoveCharacter
{
	GENERATED_BODY()
		ATrainer();
private:

	UPROPERTY(VisibleAnywhere)
	class UProjectilePool* ProjectilePool;
	UPROPERTY(Category = Weapon, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Weapon;
	UPROPERTY(Category = Weapon, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Arrow;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool CatchString;
	UPROPERTY(Replicated, Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AMoose* RidingMoose;
protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	void BeginPlay()override;
	//FORCEINLINE void SetTarget(ACharacter* Character) { Target = Character; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetWeaponLocation() const { return Weapon->GetSocketLocation("root_bow"); };

	UFUNCTION(BlueprintPure)
	FORCEINLINE USkeletalMeshComponent* GetWeapon() const { return Weapon; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE UStaticMeshComponent* GetArrow() const { return Arrow; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetCatchString() const { return CatchString; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE class AMoose* GetRidingMoose() const { return RidingMoose; };

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Tick(float DeltaTime)override;
	class AProjectile* GetArrow(FVector Location, FRotator Rotation);

	UFUNCTION(Server, Reliable)
	void Riding();

	UFUNCTION()
	void SetCatchString(bool IsCatch);

	void Jump()override;
	void MoveUpDown(float value);
	void MoveLeftRight(float value);
	void StartAttack();
	void EndAttack();

};
