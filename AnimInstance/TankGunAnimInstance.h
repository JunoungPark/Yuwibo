// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TankGunAnimInstance.generated.h"

UENUM()
enum class Location : uint8
{
	Center,
	Left,
	Right

};
/**
 * 
 */
UCLASS()
class YUWIBO_API UTankGunAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	Location Loc;
	float Delta;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		bool IsAttack;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* Gun;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		class ATank* Character; 
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		double LookRollAngle;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		double LookYawAngle;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	    double LookPitchAngle;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FVector TargetLocation;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool LookSky;
public:
	UFUNCTION()
	FORCEINLINE void SetOnOffAttack(bool IsTrue, bool IsLookSky, FVector Vector) { IsAttack = IsTrue, LookSky = IsLookSky, TargetLocation = Vector; }
	FORCEINLINE void SetLocation(Location NewLoc) { Loc = NewLoc; }
	UFUNCTION()
		void AnimNotify_LeftGunAttack();
	UFUNCTION()
		void AnimNotify_RightGunAttack();
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void UpdateLookAngles(FRotator Rotator, float DeltaSeconds);
};
