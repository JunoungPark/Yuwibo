// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "TankAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UTankAnimInstance : public UFourFootAnimInstance
{

	friend void UTankAnimComponent::OnRep_UpdateAnimationValues();

	friend void UTankLv3AnimComponent::OnRep_UpdateAnimationValues();

	GENERATED_BODY()
private:
	
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class ATank* Tank; 
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ATankLv3* TankLv3=nullptr;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool Mode; 
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsGLauncherMode; 
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool Result;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector AttackHitResult;
public:
	DECLARE_EVENT_ThreeParams(UTankAnimInstance, FOnOffAttackEvent, bool, bool, FVector)
	FOnOffAttackEvent OnOffAttack;

	DECLARE_EVENT_TwoParams(UTankAnimInstance, FOnGLauncherAttackEvent, bool, FVector)
	FOnGLauncherAttackEvent OnGLauncherAttack;
private:
	UFUNCTION()
		void AnimNotify_ToRoll();
	UFUNCTION()
		void AnimNotify_ToSpider();
	UFUNCTION()
	void AnimNotify_LandStart();
	UFUNCTION()
	void AnimNotify_LandEnd();
	UFUNCTION()
	void AnimNotify_Roll();
public:
	virtual void NativeInitializeAnimation()override;

	void FootIk(struct FFourFootIkStruct FootStruct);

	void SetIsAttack(bool IsTrue);

	void GunAttack(bool IsTrue, bool IsLookSky, FVector Vector);

	UFUNCTION()
	void GLauncherAttack();
};
