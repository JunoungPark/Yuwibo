// Fill out your copyright notice in the Description page of Project Settings.


#include "Trainer.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Moose.h"
#include "Yuwibo/Actor/ItemBox.h"
#include "Yuwibo/Animinstance/TrainerAnimInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/Animinstance/BowAnimInstance.h"
#include "Yuwibo/Actor/Projectile/ProjectilePool.h"
#include "Yuwibo/Actor/Projectile/Arrow.h"
#include "Net/UnrealNetwork.h"	
#include "Engine/OverlapResult.h"
ATrainer::ATrainer()
{
	CharacterName = TEXT("Trainer");

	SetComponent<UAIControllComponent>(Component::AIControll);

	SetComponent<UTwoFootIkComponent>(Component::FootIk)->BindSocket("foot_l", "foot_r");

	ProjectilePool = CreateDefaultSubobject<UProjectilePool>(FName("ProjectilePool"));

	ConstructorHelpers::FClassFinder<UTrainerAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Trainer/TrainerAnimBlueprint.TrainerAnimBlueprint_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ModularRogue/CharacterParts_Rogue/Meshes/UE5_Mannequin_meta/Male/SK_m_tal_nrw_rogue_dress_combined.SK_m_tal_nrw_rogue_dress_combined'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -87.f), FRotator(0.f, -90.f, 0.f));
	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SSM(TEXT("SkeletalMesh'/Game/ModularRogue/CharacterParts_Rogue/Meshes/Weapons/SK_wp_bow_simple.SK_wp_bow_simple'"));
	if (SSM.Succeeded())
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
		Weapon->SetSkeletalMesh(SSM.Object);
		Weapon->SetupAttachment(GetMesh(), "weapon_lSocket");
		ConstructorHelpers::FClassFinder<UBowAnimInstance> SAB(TEXT("AnimBlueprint'/Game/BlueprintClass/Trainer/BowAnimBlueprint.BowAnimBlueprint_C'"));
		if (SAB.Succeeded())
		{
			Weapon->SetAnimInstanceClass(SAB.Class);
		}
		ConstructorHelpers::FObjectFinder<UStaticMesh> STM(TEXT("StaticMesh'/Game/ModularRogue/CharacterParts_Rogue/Meshes/Weapons/SM_wp_arrow_01.SM_wp_arrow_01'"));
		if (STM.Succeeded())
		{
			Arrow = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
			Arrow->SetStaticMesh(STM.Object);

			Arrow->SetupAttachment(Weapon, "bow_stringSocket");

			Arrow->SetCollisionProfileName("NoCollision");
		}
		CatchString = true;
	}

}

void ATrainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATrainer, RidingMoose);
}

void ATrainer::BeginPlay()
{
	Super::BeginPlay();

	GetComponent<UTwoFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction(GetMesh()->GetAnimInstance(), FName("FootIk"));

	GetComponent<UMoveComponent>(Component::Move)->OnOffAttack.AddUFunction(this, FName("SetCatchString"));

	ProjectilePool->InitializePool<AArrow>(this);
}

void ATrainer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, GetComponent<UMoveComponent>(Component::Move), &UMoveComponent::SetAttack<true>);
	PlayerInputComponent->BindAxis(TEXT("MoveUpDown"), this, &ATrainer::MoveUpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftRight"), this, &ATrainer::MoveLeftRight);

	PlayerInputComponent->BindAction(TEXT("SpecialFeatures2"), EInputEvent::IE_Pressed, this, &ATrainer::Riding);
}

void ATrainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority() && !IsLocallyControlled())return;

	if (!Cast<UTrainerAnimInstance>(GetMesh()->GetAnimInstance())->GetIsRiding())
	{
		//FootIk();
	}
	if (RidingMoose)
	{

		SetActorLocation(RidingMoose->GetMesh()->GetSocketLocation("spine_2Socket"));
		//FRotator MooseRotator = RidingMoose->GetMesh()->GetSocketRotation("spine_2Socket");
		//RideMoose->SetActorRotation(FRotator(TrainerRotator.Roll, RideMoose->GetActorRotation().Yaw, 0.));

		//SetActorRotation(FRotator(GetActorRotation().Pitch, MooseRotator.Yaw, GetActorRotation().Roll));
		//RootYaw = FMath::FInterpTo(RootYaw, 0, DeltaSeconds, 10);

		//MovingRotation = Character->GetActorRotation();
		//FinalRotation = MovingRotation;



		
	}
}

AProjectile* ATrainer::GetArrow(FVector Location, FRotator Rotation)
{
	return ProjectilePool->GetProjectile<AArrow>(Location, Rotation);
}

void ATrainer::Riding_Implementation()
{
	if (!RidingMoose)
	{
		UWorld* World = GetWorld();
		FVector Center = GetActorLocation();
		float SearchDistance = 500.f;

		if (World != nullptr)
		{
			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams QueryParams(NAME_None, false, this);

			bool bResult = World->OverlapMultiByChannel(
				OverlapResults,
				Center,
				FQuat::Identity,
				ECollisionChannel::ECC_Visibility,
				FCollisionShape::MakeSphere(SearchDistance),
				QueryParams);

			if (bResult)
			{
				AMoose* RideMoose = nullptr;
				int MooseDistance;

				for (auto& OverlapResult : OverlapResults)
				{
					AMoose* Moose = Cast<AMoose>(OverlapResult.GetActor());
					if (Moose && Moose->Owner == this)
					{
						if (RideMoose == nullptr)
						{
							RideMoose = Moose;
							MooseDistance = GetDistanceTo(Moose);
						}
						else
						{
							if (MooseDistance < GetDistanceTo(Moose))
							{
								RideMoose = Moose;
								MooseDistance = GetDistanceTo(Moose);
							}
						}
					}


				}

				if (RideMoose)
				{
					Cast<UTrainerAnimInstance>(GetMesh()->GetAnimInstance())->Ride(RideMoose);
					RidingMoose = RideMoose;
				}


			}
		}
	}
	else
	{
		Cast<UTrainerAnimInstance>(GetMesh()->GetAnimInstance())->Ride(nullptr);
		RidingMoose = nullptr;
	}
}

void ATrainer::SetCatchString(bool IsCatch)
{
	CatchString = !IsCatch;
	Arrow->SetVisibility(!IsCatch);
}

void ATrainer::Jump()
{
	UTrainerAnimInstance* AnimInstance = Cast<UTrainerAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance->GetIsRiding())
	{
		Super::Jump();
	}
	else
	{
		AnimInstance->MooseJump();
	}
}

void ATrainer::MoveUpDown(float value)
{
	if (auto Move = GetComponent<UMoveComponent>(Component::Move))
	{
		if (!RidingMoose)
		{
			GetComponent<UMoveComponent>(Component::Move)->MoveUpDown(value);
		}
		else
		{
			RidingMoose->MoveUpDown(value);
			Move->SetForwardValue(value);
		}
	}
}

void ATrainer::MoveLeftRight(float value)
{
	if(auto Move = GetComponent<UMoveComponent>(Component::Move))
	{
		if (!RidingMoose)
		{
			Move->MoveLeftRight(value);
		}
		else
		{
			RidingMoose->MoveLeftRight(value);
			Move->SetRightValue(value);

		}
	}

}

void ATrainer::StartAttack()
{
	if (!GetIsAttack())
	{
		GetComponent<UMoveComponent>(Component::Move)->SetAttack<true>();
	}
}

void ATrainer::EndAttack()
{
	GetComponent<UMoveComponent>(Component::Move)->SetAttack<false>();
	CatchString = true;
}
