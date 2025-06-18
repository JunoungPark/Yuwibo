// Fill out your copyright notice in the Description page of Project Settings.


#include "AiComponent.h"
#include "HealthComponent.h"
#include "Yuwibo/Controller/BaseAIController.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Controller/PlayerControllAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/YuwiboGameModeBase.h"
#include "Yuwibo/ATimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
void UAiComponent::OverlapCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetOwner()->HasAuthority()) return;

	if (!Character) return;

	if (!OtherComp->IsA(UShapeComponent::StaticClass()))
	{
		ABaseCharacter* OverlapCharacter = Cast<ABaseCharacter>(OtherActor);
		if (OverlapCharacter)
		{
			auto Health = OverlapCharacter->GetComponent<UHealthComponent>(Component::Health);
			if (Character->Owner == OverlapCharacter || Character->Owner == OverlapCharacter->Owner || Health->GetCurrentHealth() <= 0);
			else
			{
				Characters.Emplace(OverlapCharacter);
				Health->AddRelatedComponents(this);
				if (!Target) Target = OverlapCharacter;
			}
		}
	}
}

void UAiComponent::OverlapEndCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!GetOwner()->HasAuthority()) return;

	if (!OtherComp->IsA(UShapeComponent::StaticClass()))
	{
		ABaseCharacter* OverlapCharacter = Cast<ABaseCharacter>(OtherActor);
		if (OverlapCharacter)
		{
			Characters.Remove(OverlapCharacter);
			OverlapCharacter->GetComponent<UHealthComponent>(Component::Health)->RemoveRelatedComponents(this);

			if (OverlapCharacter == Target)
			{
				Target = nullptr;
				if (Characters.Num())
				{
					for (auto& Char : Characters)
					{
						if (Target == nullptr || Character->GetDistanceTo(Char) < Character->GetDistanceTo(Target))
							Target = Char;
					}
				}
			}
		}
	}
}

void UAiComponent::UpdateTarget()
{
	if (Characters.Num() >= 2 && ((OwnerComponent && OwnerComponent->GetLockOn() == false) || !OwnerComponent))
	{
		for (auto& Char : Characters)
		{
			if (Character->GetDistanceTo(Char) < Character->GetDistanceTo(Target))
				Target = Char;
		}
	}
	else if (Characters.Num() == 1&&LowpriorityCharacters.Contains(Target)) Target = Characters[0];
	else if (!Characters.Num() && LowpriorityCharacters.Num())
	{
		for (int i = 0; i < LowpriorityCharacters.Num(); )
		{
			if (auto CurrentChar = Cast<ABaseCharacter>(LowpriorityCharacters[i]))  // 방어적 체크 (null 캐스팅 방지)
			{
				if (Character->GetDistanceTo(CurrentChar) > 5000.f)
				{
					CurrentChar->GetComponent<UHealthComponent>(Component::Health)->RemoveRelatedComponents(this);

					if (CurrentChar == Target) Target = nullptr;

					LowpriorityCharacters.Remove(CurrentChar);

					continue;
				}

				++i;
			}
			else   LowpriorityCharacters.RemoveAt(i);
		}

		if (!LowpriorityCharacters.Num()) return;

		for (auto& Char : LowpriorityCharacters)
		{
			if (!Target || Character->GetDistanceTo(Char) < Character->GetDistanceTo(Target))
				Target = Char;
		}
	}
}

void UAiComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Character) return;

	if (auto Owner = Cast<ABaseCharacter>(Character->Owner))
	{
		OwnerComponent = Owner->GetComponent<UAIControllComponent>(Component::AIControll);
	}

	if (!GetOwner()->HasAuthority()) return;

	if (auto Mode = Cast<AYuwiboGameModeBase>(UGameplayStatics::GetGameMode(this))) Mode->GetTimer()->OnTickEvent.AddUFunction(this, FName("UpdateTarget"));
}

UAiComponent::UAiComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetSphereRadius(2000.f);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &UAiComponent::OverlapCharacter);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &UAiComponent::OverlapEndCharacter);

	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
}

ACharacter* UAiComponent::GetTarget()
{
	if (OwnerComponent && OwnerComponent->GetLockOn()) return OwnerComponent->GetTarget();
	return Target;
}

bool UAiComponent::GetCheckOwnerLockOnAndTarget()
{
	return OwnerComponent ? OwnerComponent->GetLockOn() && GetTarget() : false;
}

void UAiComponent::AddLowpriorityCharacters(ACharacter* NewCharacter)
{
	auto Health = Cast<ABaseCharacter>(NewCharacter)->GetComponent<UHealthComponent>(Component::Health);
	if (Character->Owner == NewCharacter || Character->Owner == NewCharacter->Owner || Health->GetCurrentHealth() <= 0);
	else if(!LowpriorityCharacters.Contains(NewCharacter))
	{
		LowpriorityCharacters.Emplace(NewCharacter);
		Health->AddRelatedComponents(this);
		if (!Target) Target = NewCharacter;
	}
}

void UAiComponent::RemoveCharacter(ACharacter* NewCharacter)
{
	Characters.Remove(NewCharacter); 
	LowpriorityCharacters.Remove(NewCharacter);

	if (Target == NewCharacter)
	{
		Target = nullptr; 

		if (!Characters.Num())
		{
			if (LowpriorityCharacters.Num())
			{
				LowpriorityCharacters.RemoveAll([&](const auto& Target)
					{
						return Character->GetDistanceTo(Target) > 5000.f;
					});

				if (!LowpriorityCharacters.Num()) return;

				for (auto& Char : LowpriorityCharacters)
				{
					if (!Target || Character->GetDistanceTo(Char) < Character->GetDistanceTo(Target))
						Target = Char;
				}
			}

			return;
		}

		for (auto& Char : Characters)
		{
			if (!Target || Character->GetDistanceTo(Char) < Character->GetDistanceTo(Target))
				Target = Char;
		}
	}
}

void UAiComponent::Bind(ACharacter* NewCharacter)
{
	Character = NewCharacter;
	
	//Character->AIControllerClass = APlayerControllAIController::StaticClass();
	SphereComponent->SetupAttachment(Character->GetCapsuleComponent());
}

// Sets default values for this component's properties
UEnemyComponent::UEnemyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UEnemyComponent::Bind(ACharacter* NewCharacter)
{
	Character = NewCharacter;

	Character->AIControllerClass = ABaseAIController::StaticClass();
	Character->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

UAIControllComponent::UAIControllComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetSphereRadius(2000.f);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &UAIControllComponent::OverlapCharacter);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &UAIControllComponent::OverlapEndCharacter);

	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
}

void UAIControllComponent::OverlapCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetOwner()->HasAuthority()) return;

	if (!OtherComp->IsA(UShapeComponent::StaticClass()))
	{
		ACharacter* OverlapCharacter = Cast<ACharacter>(OtherActor);
		if (OverlapCharacter && OverlapCharacter->Owner != Character)
		{
			Characters.Emplace(OverlapCharacter);
		}
	}
}

void UAIControllComponent::OverlapEndCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!GetOwner()->HasAuthority()) return;

	if (!OtherComp->IsA(UShapeComponent::StaticClass()))
	{
		ACharacter* OverlapCharacter = Cast<ACharacter>(OtherActor);
		if (OverlapCharacter)
		{
			if (OverlapCharacter == Target)
			{
				ResetTarget();
			}
			Characters.Remove(OverlapCharacter);
		}
	}
}

void UAIControllComponent::SetTarget(ACharacter* NewCharacter)
{
	if (FVector::Distance(Character->GetActorLocation(), NewCharacter->GetActorLocation()) <= SphereComponent->GetScaledSphereRadius()) Target = NewCharacter;
}

void UAIControllComponent::Bind(ACharacter* NewCharacter)
{
	Character = NewCharacter;

	SphereComponent->SetupAttachment(Character->GetCapsuleComponent());
}

