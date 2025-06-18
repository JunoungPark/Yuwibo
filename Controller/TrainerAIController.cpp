// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainerAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ATrainerAIController::ATrainerAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/BlueprintClass/PlayerControllAI/TrainerControllBehaviorTree.TrainerControllBehaviorTree'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("BlackboardData'/Game/BlueprintClass/PlayerControllAI/PlayerControllBlackboardData.PlayerControllBlackboardData'"));
	if (BB.Succeeded())
	{
		BlackboardData = BB.Object;
	}
}

void ATrainerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	auto BlackboardComponent = GetBlackboardComponent();

	if (UseBlackboard(BlackboardData, BlackboardComponent))
	{
		if (RunBehaviorTree(BehaviorTree))
		{
		}
	}
}

void ATrainerAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
