// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAnimInstance.h"
#include "VampireAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UVampireAnimInstance : public UTwoFootEnemyAnimInstance
{
	GENERATED_BODY()
public: 
	void AnimNotify_EndMontageMove()override;
};
