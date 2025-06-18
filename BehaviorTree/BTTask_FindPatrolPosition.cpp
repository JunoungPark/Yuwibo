// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPosition.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTTask_FindPatrolPosition::UBTTask_FindPatrolPosition()
{
	NodeName = TEXT("FindPatrolPosition");
}

EBTNodeResult::Type UBTTask_FindPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (!OwnerComp.GetBlackboardComponent()) return EBTNodeResult::Failed;
	//Todo
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn != nullptr)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld()); 
		if (NavSystem != nullptr)
		{
			FNavLocation RandomLocation;
			if (NavSystem->GetRandomPointInNavigableRadius(OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("SpawnLocation")), 1000.f, RandomLocation))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("RandomPosition")), RandomLocation.Location);

				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}