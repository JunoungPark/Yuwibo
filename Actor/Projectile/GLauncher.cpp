// Fill out your copyright notice in the Description page of Project Settings.


#include "GLauncher.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectilePool.h"
#include "Yuwibo/Character/TankLv3.h"
#include "kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
AGLauncher::AGLauncher()
{
	PrimaryActorTick.bCanEverTick = true;


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(28.5, 14.1, 15));
	//BoxComponent->SetRelativeScale3D(FVector(6.));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGLauncher::OverlapGLauncher);
	BoxComponent->OnComponentHit.AddDynamic(this, &AGLauncher::HitGLauncher);
	BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
	BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel1);  // �Ѿ� ���� ä��
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	//BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Mech_Constructor_Spiders/Meshes/Projectile_Grenade_1.Projectile_Grenade_1'"));
	if (SM.Succeeded())
	{
		StaticMesh->SetStaticMesh(SM.Object);
		StaticMesh->SetRelativeLocationAndRotation((FVector(-4.5, 0., 0.)), FRotator(0., -90., 0.));
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS(TEXT("NiagaraSystem'/Game/ExplosionsMegaPack/Niagara/Big/NS_Explosion_Big_1.NS_Explosion_Big_1'"));
	if (NS.Succeeded())
	{
		Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleSystemComponent"));
		Niagara->SetAsset(NS.Object);
		Niagara->SetupAttachment(BoxComponent);

	}

	ProjectileMovementComponent->SetUpdatedComponent(BoxComponent);
	ProjectileMovementComponent->InitialSpeed = 24000.f;
	ProjectileMovementComponent->MaxSpeed = 24000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AGLauncher::Disappear()
{
	Super::Disappear();
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	OnOverlapOrHitEnd();
}

void AGLauncher::OverlapGLauncher(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap"));
	if (!OtherComp->IsA(UShapeComponent::StaticClass()) && !OtherActor->IsA(AGLauncher::StaticClass()) && OtherActor != Owner)
	{
		Attack();
	}
}

void AGLauncher::HitGLauncher(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Hit"));
	if (!OtherActor->IsA(AGLauncher::StaticClass()) && OtherActor != Owner)
	{
		Attack();
	}
}
void AGLauncher::BeginPlay()
{
	OnOverlapOrHitEnd();

	Super::BeginPlay();

}
void AGLauncher::Activate()
{

	Super::Activate();

	ProjectileMovementComponent->SetUpdatedComponent(BoxComponent);

}

void AGLauncher::Attack()
{
	ProjectileMovementComponent->StopMovementImmediately();
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnOverlapOrHit();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, this);
	QueryParams.AddIgnoredActor(Owner);

	if (auto bResult = GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(250.f), QueryParams))
	{
		TSet<ABaseCharacter*> Characters;

		for (auto& OverlapResult : OverlapResults)
		{
			if (OverlapResult.GetComponent()->IsA(UShapeComponent::StaticClass())) continue;

			auto Character = Cast<ABaseCharacter>(OverlapResult.GetActor());
			if (Character && !Characters.Contains(Character))
			{
				Characters.Emplace(Character);
				UGameplayStatics::ApplyDamage(Character, 10.f, GetInstigator()->Controller, this, UDamageType::StaticClass());

			}

		}
		DrawDebugSphere(GetWorld(), GetActorLocation(), 250.f, 12, FColor::Green, false, 5.0f);
	}
	else
		DrawDebugSphere(GetWorld(), GetActorLocation(), 250.f, 12, FColor::Red, false, 5.0f);

	FTimerHandle DestoyTimerHandle;
	GetWorldTimerManager().SetTimer(DestoyTimerHandle, [this]()
		{
			Disappear(); // ȣ���� �Ű������� �ִ� �Լ�
		}, 6.0f, false);
}

void AGLauncher::OnOverlapOrHit_Implementation()
{
	Niagara->ActivateSystem();
	StaticMesh->SetHiddenInGame(true);
}


void AGLauncher::OnOverlapOrHitEnd_Implementation()
{
	Niagara->Deactivate();
	StaticMesh->SetHiddenInGame(false);
}
