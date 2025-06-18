// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TankBackGunAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UTankBackGunAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsOn = false;
public:

	DECLARE_EVENT_OneParam(UTankBackGunAnimInstance, FOnEvent, bool)
	FOnEvent On;
private:

	UFUNCTION()
	void AnimNotify_On();
	UFUNCTION()
	void AnimNotify_Off();
public:
	FORCEINLINE bool GetIsOn() { return IsOn; };
	FORCEINLINE void SetIsOn(bool IsTrue) { IsOn = IsTrue; };
};
