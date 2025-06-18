// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "SoldierAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API USoldierAnimInstance : public UTwoFootAnimInstance
{
	friend void USoldierAnimComponent::OnRep_UpdateAnimationValues();

	GENERATED_BODY()
private:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator HeadRotator;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator HandRotator;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CameraMovingYaw;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CameraFinalYaw;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BlendWeight;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsLeftTurn = false;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsRightTurn = false;
public:
    void AnimNotify_EndAttack()override;
	//void NativeUpdateAnimation(float DeltaSeconds)override;


	//void CharacterTurn(float DeltaSeconds)override;
};
