// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlayerControllAIController.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API APlayerControllAIController : public AAIController
{
	GENERATED_BODY()

	APlayerControllAIController();
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;
	UPROPERTY()
	class UBlackboardData* BlackboardData;

	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
};
