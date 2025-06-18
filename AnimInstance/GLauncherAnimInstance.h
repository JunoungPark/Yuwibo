// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GLauncherAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UGLauncherAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:

	class ATankLv3* Character;
	USkeletalMeshComponent* GLauncher;

	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FTimerHandle FireDelayTimer;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsPack = true;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool LookSky = true;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector TargetLocation;
public:
	UGLauncherAnimInstance();
	UFUNCTION()
	bool GetIsPack() { return IsPack; };
	UFUNCTION()
	void SetIsPack(bool IsTrue) { IsPack = IsTrue; };

	UFUNCTION()
	void Attack(bool IsLookSky, FVector Vector);

	void NativeInitializeAnimation() override;
	
	UFUNCTION()
	void AnimNotify_StartPack();

	UFUNCTION()
	void AnimNotify_Pack();

	UFUNCTION()
	void AnimNotify_UnPack();

	UFUNCTION()
	void AnimNotify_GLauncherAttack();

};
