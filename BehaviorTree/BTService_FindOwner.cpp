// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindOwner.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Yuwibo/Component/AiComponent.h"
UBTService_FindOwner::UBTService_FindOwner()
{
	NodeName = TEXT("FindOwner");
	Interval = 0.f;
}

void UBTService_FindOwner::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn != nullptr)
	{
		ABaseCharacter* Owner =Cast<ABaseCharacter>(CurrentPawn->Owner);
		if (Owner)
		{
			if (UAIControllComponent* Controll = Owner->GetComponent<UAIControllComponent>(Component::AIControll))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Owner")), Owner);

				bool LockOn = Controll->GetLockOn();

				if (LockOn)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Controll->GetTarget());
					if (Controll->GetTarget() != nullptr)
					{

						OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("TargetDistance")), CurrentPawn->GetDistanceTo(Controll->GetTarget()));

					}
					else
					{
						if (OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("TargetLocation"))) != Owner->GetTargetLocation())
						{
							OwnerComp.RestartTree();
						}
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("TargetLocation")), Owner->GetTargetLocation());

					}
				}
				if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName(TEXT("LockOn"))) != LockOn)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("LockOn")), LockOn);
					OwnerComp.RestartTree();
				}
			}
		}
	}
}
