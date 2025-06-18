// Fill out your copyright notice in the Description page of Project Settings.


#include "Vampire.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Components/CapsuleComponent.h"
#include "kismet/GameplayStatics.h"
#include "Yuwibo/Animinstance/VampireAnimInstance.h"
#include "Yuwibo/Component/MoveComponent.h"
AVampire::AVampire()
{
	SetTeamId(FGenericTeamId(101));

	CharacterName = "Vampire";
	
	GetComponent<ULookComponent>(Component::Look)->SetAttackLookMove();

	SetComponent<UEnemyComponent>(Component::Enemy);

	SetComponent<UTwoFootIkComponent>(Component::FootIk)->BindSocket("foot_l", "foot_r");

	GetCapsuleComponent()->SetCapsuleHalfHeight(94.f);

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AVampire::HitBody);

	ConstructorHelpers::FClassFinder<UVampireAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/Vampire/VampireAB.VampireAB_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Vampire/Mesh/SK_Vampire.SK_Vampire'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -94.f), FRotator(0.f, -90.f, 0.f));
	}
}

void AVampire::HitBody(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetIsAttack() && OtherActor)
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		auto Enemy = GetComponent<UEnemyComponent>(Component::Enemy);
		if (Character && Character != Owner && !Enemy->FindCharacter(Character))
		{
			Enemy->AddAttackCharacter(Character);
			UGameplayStatics::ApplyDamage(Character, 10.f, GetInstigator()->Controller, this, UDamageType::StaticClass());
		}
		else
		{

			UE_LOG(LogTemp, Log, TEXT("Fail"));
		}

	}
}

void AVampire::BeginPlay()
{
	Super::BeginPlay();

	GetComponent<UTwoFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction(GetMesh()->GetAnimInstance(), FName("FootIk"));

}

//void AVampire::LookLeftRight(float value)
//{
//	if (!IsAttack)
//		Super::LookLeftRight(value);
//}
