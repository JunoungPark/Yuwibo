// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Animinstance/BaseEnemyAnimInstance.h"
#include "Yuwibo/ATimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/YuwiboGameModeBase.h"
ABaseAIController::ABaseAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/BlueprintClass/Enemy/EnemyBehaviorTree.EnemyBehaviorTree'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("BlackboardData'/Game/BlueprintClass/Enemy/EnemyBlackboardData.EnemyBlackboardData'"));
	if (BB.Succeeded())
	{
		BlackboardData = BB.Object;
	}

	
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("Perception"));

	UAISenseConfig_Sight* Config_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Config_Sight");
	
	Config_Sight->SightRadius = 3000.f;
	Config_Sight->LoseSightRadius = 3500.f;
	Config_Sight->PeripheralVisionAngleDegrees = 90.f;
	Config_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Config_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	//Config_Sight->DetectionByAffiliation.bDetectEnemies = false;
	Config_Sight->AutoSuccessRangeFromLastSeenLocation = 1.f;
	//Config_Sight->GetDebugColor();
	Config_Sight->SetMaxAge(0.f);
	
	GetPerceptionComponent()->SetDominantSense(*Config_Sight->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*Config_Sight);

	UAISenseConfig_Hearing* Config_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("Config_Hearing");
	
	Config_Hearing->HearingRange = 3000.f;
	Config_Hearing->DetectionByAffiliation.bDetectFriendlies = false;
	Config_Hearing->DetectionByAffiliation.bDetectNeutrals = false;
	Config_Hearing->DetectionByAffiliation.bDetectEnemies = true;
	//Config_Hearing->GetDebugColor();
	Config_Hearing->SetMaxAge(3.f);
	
	GetPerceptionComponent()->ConfigureSense(*Config_Hearing);

	UAISenseConfig_Damage* Config_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>("Config_Damage");

	Config_Damage->SetMaxAge(3.f);

	GetPerceptionComponent()->ConfigureSense(*Config_Damage);

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::Reaction);

}

void ABaseAIController::Reaction(AActor* actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISenseConfig_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (!GetBlackboardComponent()->GetValueAsObject(FName("Target")))
			{
				GetBlackboardComponent()->SetValueAsObject(FName("Target"), actor);
				OnSight.ExecuteIfBound(actor);
			}
		}
		else
		{
			//GetBlackboardComponent()->SetValueAsObject(FName("Target"), nullptr);
			//OnSight.ExecuteIfBound(nullptr);
		}
	}

	if (Stimulus.Type == UAISense::GetSenseID<UAISenseConfig_Hearing>())
	{
		//UE_LOG(LogTemp, Log, TEXT("%d"), Stimulus.WasSuccessfullySensed());
		if (Stimulus.WasSuccessfullySensed())
		{
			OnNoise.ExecuteIfBound(true, Stimulus.StimulusLocation); 
			GetBlackboardComponent()->SetValueAsBool(FName("IsLook"), true);
		}
		else
		{
			OnNoise.ExecuteIfBound(false, FVector());
			GetBlackboardComponent()->SetValueAsBool(FName("IsLook"), false); 
		}
	}

	if (Stimulus.Type == UAISense::GetSenseID<UAISenseConfig_Damage>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (!GetBlackboardComponent()->GetValueAsObject(FName("Target")))
			{
				GetBlackboardComponent()->SetValueAsObject(FName("Target"), actor);
				OnSight.ExecuteIfBound(actor);
			}
		}
		else
		{
			//GetBlackboardComponent()->SetValueAsObject(FName("Target"), nullptr);
			//OnSight.ExecuteIfBound(nullptr);
		}
	}
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto Char = Cast<ABaseCharacter>(GetPawn());
	SetGenericTeamId(Char->GetGenericTeamId());

	auto Anim = Char->GetComponent<UAnimComponent>(Component::Anim);
	OnNoise.BindUFunction(Anim, FName("SetLook"));
	OnSight.BindUFunction(Anim, FName("SetTarget"));

	auto BlackboardComponent = GetBlackboardComponent();

	if (UseBlackboard(BlackboardData, BlackboardComponent))
	{
		if (RunBehaviorTree(BehaviorTree))
		{
			GetBlackboardComponent()->SetValueAsVector(FName("SpawnLocation"), Char->GetActorLocation());
		}
	}
}

void ABaseAIController::OnUnPossess()
{
	Super::OnUnPossess();
}


ETeamAttitude::Type ABaseAIController::GetTeamAttitudeTowards(const AActor& Other) const
{

	if (const ABaseCharacter* OtherTeamAgent = Cast<const ABaseCharacter>(&Other))
	{
		if (GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId())
		{
			return ETeamAttitude::Friendly;
		}
		else
		{
			return ETeamAttitude::Hostile;
		}
	}
	else
		return ETeamAttitude::Neutral;
}
