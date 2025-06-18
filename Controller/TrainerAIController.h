// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TrainerAIController.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ATrainerAIController : public AAIController
{
	GENERATED_BODY()

	ATrainerAIController();
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;
	UPROPERTY()
	class UBlackboardData* BlackboardData;

	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
};
