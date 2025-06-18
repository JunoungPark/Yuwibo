// Fill out your copyright notice in the Description page of Project Settings.


#include "RockMonster.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Yuwibo/Controller/RockMonsterAIController.h"
#include "Yuwibo/Animinstance/RockMonsterAnimInstance.h"
ARockMonster::ARockMonster()
{
	CharacterName = "RockMonster";

	GetComponent<ULookComponent>(Component::Look)->SetAttackLookMove();

	SetComponent<UEnemyComponent>(Component::Enemy);

	SetComponent<UTwoFootIkComponent>(Component::FootIk)->BindSocket("b_foot_L", "b_foot_R");

	GetComponent<UMoveComponent>(Component::Move)->SetCrouchCanMove();

	AIControllerClass = ARockMonsterAIController::StaticClass();

	ConstructorHelpers::FClassFinder<URockMonsterAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/RockMonster/RockMonsterAB.RockMonsterAB_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/HB_RockMonster/SkeletalMesh/HB_RockMonster.HB_RockMonster'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -86.f));
	}
}

void ARockMonster::BeginPlay()
{
	Super::BeginPlay();

	GetComponent<UTwoFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction(GetMesh()->GetAnimInstance(), FName("FootIk"));

}

void ARockMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UMoveComponent* Move = GetComponent<UMoveComponent>(Component::Move);
	
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, Move, &UMoveComponent::SetCrouch<true>);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, Move, &UMoveComponent::SetCrouch<false>);
}

float ARockMonster::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetComponent<UAnimComponent>(Component::Anim)->GetIsCrouch()) return 0.f;

	return Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);

}

//void ARockMonster::LookLeftRight(float value)
//{
//	if (!IsAttack)
//		Super::LookLeftRight(value);
//}
