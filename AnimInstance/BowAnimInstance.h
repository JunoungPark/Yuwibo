// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BowAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UBowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:

	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ATrainer* Character;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector StringLocation;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StringAlpha;
public:
	void NativeInitializeAnimation()override;
	void NativeUpdateAnimation(float DeltaSeconds)override;
};
