// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "GroundTurretAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UGroundTurretAnimInstance : public UFourFootAnimInstance
{
	GENERATED_BODY()
	friend void UGroundTurretAnimComponent::OnRep_UpdateAnimationValues();
private:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		class AGroundTurret* Turret;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		FRotator GunRotation;
	UFUNCTION()
		void AnimNotify_Attack();

	virtual void NativeInitializeAnimation()override;
	
	//virtual void NativeUpdateAnimation(float DeltaSeconds)override;

public:
	//void CharacterTurn(float DeltaSeconds)override;

	void FootIk(struct FFourFootIkStruct FootStruct);

	FORCEINLINE void SetGunRotation(FRotator Rotator) { GunRotation = Rotator; }

};
