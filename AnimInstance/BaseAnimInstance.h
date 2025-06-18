// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Yuwibo/Component/AnimComponent.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	friend void UAnimComponent::OnRep_UpdateAnimationValues();
protected:
	
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackMontage;
	//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//	bool RotationToMove;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool IsAttack;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	bool IsCrouch = false;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		class ABaseCharacter* Character;
	//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	//	class UCharacterMovementComponent* CharacterMovement;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool IsFalling;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		FVector Velocity;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		float GroundSpeed;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool IsMoving;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		float BlendAlpha;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		FRotator AimRotation;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		float YawOffset;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	float RootYaw = 0.f;
	//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	//	float AttackYaw;
	//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	//	FRotator MovingRotation;
	//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	//	FRotator FinalRotation;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		float ForwardValue;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		float RightValue;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool IsDead;
public:
	//UFUNCTION()
	// virtual void SetIsAttack(bool IsTrue) { IsAttack = IsTrue; }
	//UFUNCTION()
	//FORCEINLINE void SetIsCrouch(bool IsTrue) { IsCrouch = IsTrue; }
	//
	//FORCEINLINE void SetIsDead(bool IsTrue) { IsDead = IsTrue; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsAttack() const { return IsAttack; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetBlendAlpha() const { return BlendAlpha; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetGroundSpeed() const { return GroundSpeed; };

	UFUNCTION(BlueprintPure)
		FORCEINLINE UAnimMontage* GetAttackMontage() const { return AttackMontage; };

	UFUNCTION()
	virtual void AnimNotify_Search();
	UFUNCTION()
		virtual void AnimNotify_EndMontageMove();
	UFUNCTION()
		virtual void AnimNotify_EndAttack();
	UFUNCTION()
		virtual void AnimNotify_FootStep();
	virtual void NativeInitializeAnimation()override;

	//virtual void NativeUpdateAnimation(float DeltaSeconds)override;
	//
	//virtual void CharacterTurn(float DeltaSeconds);
	
	UFUNCTION()
	virtual void PlayAttackAnimation();
	
	virtual void PlayAttackAnimation(float Position);

	void DebugCapsule(float AttackRange, float CapsuleHalfHeight, FVector StartOffset = FVector(0.));
};

UCLASS()
class YUWIBO_API UTwoFootAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector HipOffset;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator LeftFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator RightFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector LeftFootEffectorLocation;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector RightFootEffectorLocation;
	
private:

	UFUNCTION()
	void FootIk(struct FTwoFootIkStruct FootStruct);
};

UCLASS()
class YUWIBO_API UFourFootAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
protected:

	
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector HipOffset;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator LeftFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator RightFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator BackLeftFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator BackRightFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector LeftFootEffectorLocation;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector RightFootEffectorLocation;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector BackLeftFootEffectorLocation;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector BackRightFootEffectorLocation;
private:

	UFUNCTION()
	void FootIk(struct FFourFootIkStruct FootStruct);

};