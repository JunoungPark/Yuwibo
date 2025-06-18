// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ResetFailMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Component/AnimComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

UBTService_ResetFailMoveTo::UBTService_ResetFailMoveTo()
{
	NodeName = TEXT("ResetFailMoveTo");
	Interval = 0.f;
}

void UBTService_ResetFailMoveTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (auto Blackboard = OwnerComp.GetBlackboardComponent())
	{
		if (Blackboard->GetValueAsBool(FName("FailMoveTo")) == false) return;

		if (auto NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			if (auto Target = Cast<AActor>(Blackboard->GetValueAsObject(FName("Target"))))
			{
				UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(GetWorld(), OwnerComp.GetAIOwner()->GetPawn()->GetTargetLocation(), Target->GetTargetLocation());

				// 경로가 존재하고, 완전히 탐색 가능한 경우
				if (!NavPath || !NavPath->IsValid() || NavPath->PathPoints.Num() <= 1 || NavPath->IsPartial()) return;

				Blackboard->SetValueAsBool(FName("FailMoveTo"), false);

				if (auto Anim = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn())->GetComponent<UEnemyAnimComponent>(Component::Anim))
				{
					Anim->SetIsFailToMove(false);
				}

				UE_LOG(LogTemp, Log, TEXT("Succeeded"));
			}

		}
	}
}
