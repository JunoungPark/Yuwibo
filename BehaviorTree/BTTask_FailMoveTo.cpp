// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FailMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Component/AnimComponent.h"

UBTTask_FailMoveTo::UBTTask_FailMoveTo()
{
	NodeName = TEXT("FailMoveTo");
}

EBTNodeResult::Type UBTTask_FailMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (auto Blackboard = OwnerComp.GetBlackboardComponent())
	{
		Blackboard->SetValueAsBool(FName("FailMoveTo"), true);

		if (auto Anim = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn())->GetComponent<UEnemyAnimComponent>(Component::Anim))
		{
			Anim->SetIsFailToMove(true);
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
