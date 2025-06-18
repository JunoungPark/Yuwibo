// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EndAttack.h"
#include "AIController.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Component/MoveComponent.h"

UBTTask_EndAttack::UBTTask_EndAttack()
{
	NodeName = TEXT("EndAttack");
}

EBTNodeResult::Type UBTTask_EndAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//Todo
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn != nullptr)
	{

		Cast<ABaseCharacter>(CurrentPawn)->GetComponent<UMoveComponent>(Component::Move)->SetAttack<false>();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
