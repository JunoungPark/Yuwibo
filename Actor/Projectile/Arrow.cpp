// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Yuwibo/Character/Trainer.h"
#include "kismet/GameplayStatics.h"
#include "Yuwibo/Controller/TrainerAIController.h"
#include "Yuwibo/Animinstance/BaseEnemyAnimInstance.h"
#include "AIController.h"
#include "Yuwibo/Character/Moose.h"
#include "Yuwibo/Component/HealthComponent.h"
#include "Yuwibo/Component/AiComponent.h"
#include "ProjectilePool.h"
// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(3.5, 3.2, 0.5));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OverlapArrow);
	BoxComponent->OnComponentHit.AddDynamic(this, &AArrow::HitArrow);
	BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/ModularRogue/CharacterParts_Rogue/Meshes/Weapons/SM_wp_arrow_01.SM_wp_arrow_01'"));
	if (SM.Succeeded())
	{
		StaticMesh->SetCollisionProfileName("NoCollision");
		StaticMesh->SetStaticMesh(SM.Object);
		StaticMesh->SetRelativeLocationAndRotation((FVector(-86., 0., 0.2)), FRotator(0., -90., 0.));

	}

	ProjectileMovementComponent->SetUpdatedComponent(BoxComponent);
	ProjectileMovementComponent->InitialSpeed = 7000.f;
	ProjectileMovementComponent->MaxSpeed = 7000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

}

void AArrow::OverlapArrow(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		//{ 
	if (!Cast<UShapeComponent>(OtherComp))
	{
		if (Cast<AMoose>(OtherActor) && Cast<AMoose>(OtherActor) == Cast<ATrainer>(Owner)->GetRidingMoose())
		{
			return;
		}
		if (!Cast<ACharacter>(OtherActor))
		{

			ProjectileMovementComponent->StopMovementImmediately();
			AttachToComponent(OtherComp, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


			FTimerHandle DestoyTimerHandle;
			GetWorldTimerManager().SetTimer(DestoyTimerHandle, [this]()
				{
					DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					ProjectilePool->ReturnProjectile(this); // ȣ���� �Ű������� �ִ� �Լ�
					BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
				}, 3.0f, false);
		}
		else
		{
			ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
			if (Character)
			{

			}
			if (UEnemyComponent* Enemy = Character->GetComponent<UEnemyComponent>(Component::Enemy))
			{
				if (Cast<ATrainer>(Character->Owner))
				{

				}
				else
				{
					Enemy->AddTrainerCount();

					UHealthComponent* Health = Character->GetComponent<UHealthComponent>(Component::Health);

					if (Enemy->GetTrainerCount() >= 5 || Health->GetCurrentHealth() <= 10.f)
					{
						Health->SetCurrentHealth(Health->GetMaxHealth());
						ATrainerAIController* AIController = GetWorld()->SpawnActor<ATrainerAIController>();
						Character->AIControllerClass = ATrainerAIController::StaticClass();
						AIController->Possess(Character);
						Character->Owner = this->Owner;
						Enemy->RemoveAttackCharacters(Cast<ACharacter>(this->Owner));
					}
					else
					{
						ATrainer* Trainer = Cast<ATrainer>(Owner);
						UAIControllComponent* Controll = Trainer->GetComponent<UAIControllComponent>(Component::AIControll);
						if (Controll->GetCharacters().Find(Cast<ACharacter>(OtherActor)) != INDEX_NONE)
						{
							Controll->SetTarget(Cast<ACharacter>(OtherActor));
						}
						UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigator()->Controller, this, UDamageType::StaticClass());
					}

					Destroy();
				}
			}
			else if (!Cast<ATrainer>(OtherActor))
			{
				ATrainer* Trainer = Cast<ATrainer>(Owner);

				UAIControllComponent* Controll = Trainer->GetComponent<UAIControllComponent>(Component::AIControll);
				if (Controll->GetCharacters().Find(Cast<ACharacter>(OtherActor)) != INDEX_NONE)
				{
					Controll->SetTarget(Cast<ACharacter>(OtherActor));
				}
				UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigator()->Controller, this, UDamageType::StaticClass());

				Destroy();
			}
		}
	}
}

void AArrow::HitArrow(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileMovementComponent->StopMovementImmediately();
	AttachToComponent(OtherComp, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DestoyTimerHandle;
	GetWorldTimerManager().SetTimer(DestoyTimerHandle, [this]()
		{
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ProjectilePool->ReturnProjectile(this); // ȣ���� �Ű������� �ִ� �Լ�
			BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
		}, 3.0f, false);
}

void AArrow::Activate()
{
	Super::Activate();

	ProjectileMovementComponent->SetUpdatedComponent(BoxComponent);
}
