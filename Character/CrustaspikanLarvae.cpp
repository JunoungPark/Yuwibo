// Fill out your copyright notice in the Description page of Project Settings.


#include "CrustaspikanLarvae.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Yuwibo/Animinstance/CrustaspikanLarvaeAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "kismet/GameplayStatics.h"
ACrustaspikanLarvae::ACrustaspikanLarvae()
{
	CharacterName = "CrustaspikanLarvae";

	GetComponent<ULookComponent>(Component::Look)->SetAttackLookMove();

	SetComponent<UEnemyComponent>(Component::Enemy);

	SetComponent<UTwoFootIkComponent>(Component::FootIk)->BindSocket("CrustaspikanLarvae_-L-Hand", "CrustaspikanLarvae_-R-Hand");

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACrustaspikanLarvae::HitBody);

	ConstructorHelpers::FClassFinder<UCrustaspikanLarvaeAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/CrustaspikanLarvae/CrustaspikanLarvaeAB.CrustaspikanLarvaeAB_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/CrustaspikanLarvae/Meshes/SK_CrustaspikanLarvae.SK_CrustaspikanLarvae'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));

	}
}

void ACrustaspikanLarvae::HitBody(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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

void ACrustaspikanLarvae::BeginPlay()
{
	Super::BeginPlay();

	GetComponent<UTwoFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction(GetMesh()->GetAnimInstance(), FName("FootIk"));
}
