// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "DroneAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UDroneAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

	friend void UDroneAnimComponent::OnRep_UpdateAnimationValues();
private:
	class ADrone* Drone;

	bool LookSky;

	FVector TargetLocation;
public:

	UFUNCTION()
	void AnimNotify_TopGunAttack();

	UFUNCTION()
	void AnimNotify_BotGunAttack();

	void NativeInitializeAnimation()override;

	//void NativeUpdateAnimation(float DeltaSeconds)override;
};
