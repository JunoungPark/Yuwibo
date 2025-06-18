// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "BodyChangerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UBodyChangerAnimInstance : public UTwoFootAnimInstance
{
	GENERATED_BODY()
	friend void UBodyChangerAnimComponent::OnRep_UpdateAnimationValues();
private:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ABodyChanger* BodyChanger;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FVector AttackHitResult;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool LookSky;
public:
	UFUNCTION()
		void AnimNotify_StartAttack();

		void AnimNotify_Search()override;
	void NativeInitializeAnimation()override;
	//void FindHUDRotation();

};
