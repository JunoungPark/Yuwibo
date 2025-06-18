// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterBall.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Yuwibo/Character/BodyChanger.h"
#include "Yuwibo/Animinstance/BodyChangerAnimInstance.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "kismet/GameplayStatics.h"
#include "ProjectilePool.h"	
#include "Engine/OverlapResult.h"
// Sets default values
AWaterBall::AWaterBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWaterBall::OverlapWaterBall);
	SphereComponent->OnComponentHit.AddDynamic(this, &AWaterBall::HitWaterBall);
	SphereComponent->SetCollisionProfileName("OverlapOnlyPawn");
	SphereComponent->SetSphereRadius(50.f);
	SphereComponent->SetRelativeScale3D(FVector(0.3));

	ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_waterBall.P_ky_waterBall'"));
	if (PS.Succeeded())
	{
		DefalutParticleSystem = PS.Object;

		ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
		ParticleComponent->SetTemplate(PS.Object);
		ParticleComponent->SetupAttachment(SphereComponent);

		ConstructorHelpers::FObjectFinder<UParticleSystem> SPS(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_waterBallHit.P_ky_waterBallHit'"));
		ParticleSystem = SPS.Object;
	}
	

	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 5000.f;
	ProjectileMovementComponent->MaxSpeed = 5000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;


}

void AWaterBall::Disappear()
{
	Super::Disappear();
	OnOverlapOrHitEnd();
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWaterBall::Attack()
{
	ProjectileMovementComponent->StopMovementImmediately();
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnOverlapOrHit();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, this);
	QueryParams.AddIgnoredActor(Owner);

	if (auto bResult = GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(200.f), QueryParams))
	{
		TSet<ABaseCharacter*> Characters;

		for (auto& OverlapResult : OverlapResults)
		{
			if (OverlapResult.GetComponent()->IsA(UShapeComponent::StaticClass())) continue;

			auto Character = Cast<ABaseCharacter>(OverlapResult.GetActor());
			if (Character && !Characters.Contains(Character))
			{
				Characters.Emplace(Character);
				UGameplayStatics::ApplyDamage(Character, 20.f, GetInstigator()->Controller, this, UDamageType::StaticClass());

			}

		}
		DrawDebugSphere(GetWorld(), GetActorLocation(), 200.f, 12, FColor::Green, false, 5.0f);
	}
	else
		DrawDebugSphere(GetWorld(), GetActorLocation(), 200.f, 12, FColor::Red, false, 5.0f);

	FTimerHandle DestoyTimerHandle;
	GetWorldTimerManager().SetTimer(DestoyTimerHandle, [this]()
		{
			Disappear(); // ȣ���� �Ű������� �ִ� �Լ�
		}, 2.0f, false);

}

void AWaterBall::OverlapWaterBall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherComp->IsA(UShapeComponent::StaticClass()) && ParticleComponent->Template == DefalutParticleSystem && OtherActor != Owner)
	{
		Attack();
	}
}

void AWaterBall::HitWaterBall(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ParticleComponent->Template == DefalutParticleSystem && OtherActor != Owner)
	{
		Attack();
	}
}

void AWaterBall::Activate()
{
	Super::Activate();

	On();

	OnOverlapOrHitEnd();

	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
}

void AWaterBall::On_Implementation()
{
	ParticleComponent->ActivateSystem();
}

void AWaterBall::OnOverlapOrHit_Implementation()
{
	ParticleComponent->SetTemplate(ParticleSystem);
	
	if (!HasAuthority())	SphereComponent->SetRelativeScale3D(FVector(2.));

}

void AWaterBall::OnOverlapOrHitEnd_Implementation()
{
	SphereComponent->SetRelativeScale3D(FVector(0.3)); 

	ParticleComponent->SetTemplate(DefalutParticleSystem);
	
}
