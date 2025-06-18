// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAnimInstance.h"
#include "RockMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API URockMonsterAnimInstance : public UTwoFootEnemyAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void AnimNotify_StartAttack(); 

	//void NativeUpdateAnimation(float DeltaSeconds)override;
};
