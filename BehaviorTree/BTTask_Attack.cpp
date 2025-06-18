// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn != nullptr)
	{
		ABaseCharacter* CurrentCharacter = Cast<ABaseCharacter>(CurrentPawn);
		if (CurrentCharacter != nullptr)
		{
			bool CanMove = CurrentCharacter->GetComponent<UMoveComponent>(Component::Move)->GetCanMove();

			OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanMove", CanMove);

			if (CanMove)
			{
				if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")) && Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")))->GetDistanceTo(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"))) <= 300.f)
				{
					CurrentCharacter->GetComponent<UMoveComponent>(Component::Move)->SetAttack<true>();

				}

			}

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
