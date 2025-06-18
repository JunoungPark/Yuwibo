// Fill out your copyright notice in the Description page of Project Settings.


#include "RockMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
ARockMonsterAIController::ARockMonsterAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/BlueprintClass/Enemy/RockMonster/RockMonsterBehaviorTree.RockMonsterBehaviorTree'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}
}