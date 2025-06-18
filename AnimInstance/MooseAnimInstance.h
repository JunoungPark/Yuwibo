// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAnimInstance.h"
#include "MooseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UMooseAnimInstance : public UFourFootEnemyAnimInstance
{
	GENERATED_BODY()
private:
	UFUNCTION()
	void MooseFootIk(struct FFourFootIkStruct FootStruct);

	//void CharacterTurn(float DeltaSeconds)override;
	//void NativeUpdateAnimation(float DeltaSeconds)override;
//public:
	//FORCEINLINE void AddRootYaw(float value) { RootYaw += value; }
};