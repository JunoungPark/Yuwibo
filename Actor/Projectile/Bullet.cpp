// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "ProjectilePool.h"
#include "kismet/GameplayStatics.h"
// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(4.4, 2.5, 2.5));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapBullet);
	BoxComponent->OnComponentHit.AddDynamic(this, &ABullet::HitBullet);
	BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
	//BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	//BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	//BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetRelativeScale3D(FVector(5.));
	StaticMesh->SetupAttachment(BoxComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/HorseAnimsetPro/Meshes/Weapons/SM_Pistol_Bullet.SM_Pistol_Bullet'"));
	if (SM.Succeeded())
	{
		StaticMesh->SetStaticMesh(SM.Object);
		StaticMesh->SetRelativeLocationAndRotation((FVector(1.3, 0., 0.)), FRotator(-90., 0., 0.));
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}

	ProjectileMovementComponent->SetUpdatedComponent(BoxComponent);
	ProjectileMovementComponent->InitialSpeed = 40000.f;
	ProjectileMovementComponent->MaxSpeed = 40000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void ABullet::OverlapBullet(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherComp->IsA(UShapeComponent::StaticClass()) && OtherActor->IsA(ACharacter::StaticClass()) && OtherActor != Owner)
	{
		ProjectilePool->ReturnProjectile(this);

		if (auto Character = Cast<ABaseCharacter>(OtherActor))
		{
			auto CurrentOwner = Cast<ABaseCharacter>(Owner);
			if (Character != CurrentOwner->Owner && Character->Owner != CurrentOwner->Owner) UGameplayStatics::ApplyDamage(Character, 5.f, GetInstigator()->Controller, this, UDamageType::StaticClass());
		}

	}
}


void ABullet::HitBullet(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != Owner)
	{
		ProjectilePool->ReturnProjectile(this);
	}
}
