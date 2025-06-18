// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

APlayerControllAIController::APlayerControllAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/BlueprintClass/PlayerControllAI/PlayerControllBehaviorTree.PlayerControllBehaviorTree'"));
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

void APlayerControllAIController::OnPossess(APawn* InPawn)
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

void APlayerControllAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
