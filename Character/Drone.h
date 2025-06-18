// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "Drone.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ADrone : public AMoveCharacter
{
	GENERATED_BODY()

	ADrone();
private:
	UPROPERTY(VisibleAnywhere)
	class UProjectilePool* ProjectilePool;
	UPROPERTY(VisibleAnywhere)
	TSet<class UNiagaraComponent*> TopMuzzles;
	UPROPERTY(VisibleAnywhere)
	TSet<class UNiagaraComponent*> BotMuzzles;

//protected:
//
//	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:

	void BeginPlay()override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class AProjectile* GetBullet(FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetTopLeftGunLocation() const { return GetMesh()->GetSocketLocation(TEXT("b_gun_top_left_end")); };
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetTopRightGunLocation() const { return GetMesh()->GetSocketLocation(TEXT("b_gun_top_right_end")); };

	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetBotLeftGunLocation() const { return GetMesh()->GetSocketLocation(TEXT("b_gun_bot_left_end")); };
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetBotRightGunLocation() const { return GetMesh()->GetSocketLocation(TEXT("b_gun_bot_right_end")); };

	void ResetTopFx();
	void ResetBotFx();

	UFUNCTION(NetMulticast, Reliable)
	void Call(float value);

	void Jump()override;
	void Fall();
	//void Tick(float DeltaTime) override;

	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
