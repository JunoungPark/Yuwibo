// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "Yuwibo/Character/Moose.h"
#include "TrainerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UTrainerAnimInstance : public UTwoFootAnimInstance
{
	GENERATED_BODY()

	friend void UTrainerAnimComponent::OnRep_UpdateAnimationValues();

private:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ATrainer* Trainer;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator BlendRotator;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector AttackHitResult;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool LookSky;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AMoose* RideMoose;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsRide;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MooseBlend;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MooseSpeed;
public:

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsRiding() const { if (RideMoose) { return true; } else { return false; } };

	FORCEINLINE void MooseJump() { RideMoose->Jump(); };

	UFUNCTION()
	virtual void AnimNotify_CatchArrow();
	UFUNCTION()
	virtual void AnimNotify_CatchString();
	UFUNCTION()
	virtual void AnimNotify_PutString();


	void NativeInitializeAnimation()override;
	//void NativeUpdateAnimation(float DeltaSeconds)override;
	void Ride(class AMoose* Moose);
};
