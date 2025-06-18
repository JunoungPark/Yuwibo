// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "Yuwibo/Component/ItemComponent.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Component/SkillComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Drone.h"
#include "Yuwibo/Animinstance/BaseAnimInstance.h"
#include "Yuwibo/Animinstance/SoldierAnimInstance.h"
#include "GroundTurret.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "kismet/GameplayStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/YuwiboGameModeBase.h"
#include "Yuwibo/ATimerManager.h"
ASoldier::ASoldier()
{
	SetTeamId(FGenericTeamId(100));

	PrimaryActorTick.bCanEverTick = false;

	CharacterName = TEXT("Soldier");

	//SetComponent<UEquipmentComponent>(Component::Equipment);

	SetComponent<UAIControllComponent>(Component::AIControll);

	//auto SphereComponent = SetComponent<UAIControllComponent>(Component::AIControll)->GetSphereComponent();
	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASoldier::OverlapCharacter);
	//SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ASoldier::OverlapEndCharacter);
	
	GetComponent<UMoveComponent>(Component::Move)->SetAttackCanMove();

	ULookComponent* Look = GetComponent<ULookComponent>(Component::Look);

	Look->GetCamera()->SetFieldOfView(125.f);
	
	SetComponent<UTwoFootIkComponent>(Component::FootIk)->BindSocket("foot_l", "foot_r");

	ConstructorHelpers::FClassFinder<USoldierAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Soldier/SoldierAB.SoldierAB_C'"));

	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/AAAModularSoldier/Modules/ModularSoldierVol_1/SK_SOF_A.SK_SOF_A'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	}

	Look->GetSpringArm()->DestroyComponent();
	Look->GetCamera()->bUsePawnControlRotation = true;
	Look->GetCamera()->SetupAttachment(GetMesh(),TEXT("headSocket"));
	
	//ConstructorHelpers::FObjectFinder<UMaterial> MT(TEXT("Material'/Game/BlueprintClass/Materials/SoldierOutLine.SoldierOutLine'"));
	//if (MT.Succeeded())
	//{
	//	Look->GetCamera()->PostProcessSettings.AddBlendable(MT.Object, 1);
	//}

	SetComponent<USkillComponent>(Component::Skill)->SetCoolTime(2.);

}

//void ASoldier::OverlapCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (!OtherComp->IsA(UShapeComponent::StaticClass()))
//	{
//		ACharacter* Character = Cast<ACharacter>(OtherActor);
//		if (Character && Character->Owner != this)
//		{
//			Character->GetMesh()->SetCustomDepthStencilValue(Character->GetMesh()->CustomDepthStencilValue + 1);
//		}
//	}
//}
//
//void ASoldier::OverlapEndCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (!OtherComp->IsA(UShapeComponent::StaticClass()))
//	{
//		ACharacter* Character = Cast<ACharacter>(OtherActor);
//		if (Character)
//		{
//			if (Character->GetMesh()->CustomDepthStencilValue & 2)
//			{
//				Character->GetMesh()->SetCustomDepthStencilValue(Character->GetMesh()->CustomDepthStencilValue - 2);
//			}
//			
//			Character->GetMesh()->SetCustomDepthStencilValue(Character->GetMesh()->CustomDepthStencilValue - 1);
//		}
//	}
//}

void ASoldier::SetTarget()
{
	if (!GetOwner()->HasAuthority()) return;

	ULookComponent* Look = GetComponent<ULookComponent>(Component::Look);
	UAIControllComponent* Controll = GetComponent<UAIControllComponent>(Component::AIControll);

	FCollisionQueryParams Params(NAME_None, false, this);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActors(Drones);
	Params.AddIgnoredActors(Turrets);
	Params.bIgnoreTouches = true;
	FHitResult Hit;

	if (GetWorld()->LineTraceSingleByChannel(Hit, Look->GetCamera()->GetComponentLocation(), Look->GetCamera()->GetComponentLocation() + GetBaseAimRotation().Vector() * 1000.f, ECollisionChannel::ECC_Visibility, Params))
	{
		if (auto Character = Cast<ACharacter>(Hit.GetActor()))
		{
			if (Character->Owner != this && Character != Controll->GetTarget())
			{
				Controll->SetTarget(Character);

				//if (!(Controll->GetTarget()->GetMesh()->CustomDepthStencilValue & 2))
				//{
				//	Controll->GetTarget()->GetMesh()->SetCustomDepthStencilValue(Controll->GetTarget()->GetMesh()->CustomDepthStencilValue + 2);
				//}

				Controll->SetLockOn(true);
			}
		}
	}
}

void ASoldier::ResetTarget_Implementation()
{
	//UAIControllComponent* Controll = GetComponent<UAIControllComponent>(Component::AIControll);
	//if (Controll->GetTarget() && Controll->GetTarget()->GetMesh()->CustomDepthStencilValue & 2)
	//{
	//	Controll->GetTarget()->GetMesh()->SetCustomDepthStencilValue(Controll->GetTarget()->GetMesh()->CustomDepthStencilValue - 2);
	//}
	GetComponent<UAIControllComponent>(Component::AIControll)->ResetTarget();
}

void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("SpecialFeatures"), EInputEvent::IE_Pressed, this, &ASoldier::ResetTarget);
	//PlayerInputComponent->BindAction(TEXT("SpecialFeatures2"), EInputEvent::IE_Pressed, this, &ASoldier::CreateDrone);
	PlayerInputComponent->BindAction(TEXT("SpecialSkill"), EInputEvent::IE_Pressed, this, &ASoldier::CreateTurret);
}

void ASoldier::BeginPlay()
{
	Super::BeginPlay();

	GetComponent<UTwoFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction(GetMesh()->GetAnimInstance(), FName("FootIk"));
	
	if (!HasAuthority()) return;

	GetComponent<UMoveComponent>(Component::Move)->OnAttack.AddUFunction(this, FName("SetTarget"));

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Drone->Owner.GetName());
	Drones.Emplace(GetWorld()->SpawnActor<ADrone>(GetMesh()->GetBoneLocation("upperarm_r") + GetActorUpVector() * 30, GetBaseAimRotation(), spawnParameters));
	//static_cast<AYuwiboGameModeBase*>(UGameplayStatics::GetGameMode(this))->GetTimer()->OnTickEvent.AddUFunction(this, FName("TargetOutline"));
}

float ASoldier::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority())
	{
		for (auto& Drone :Drones)
			Cast<ABaseCharacter>(Drone)->GetComponent<UAiComponent>(Component::AI)->AddLowpriorityCharacters(Cast<ABaseCharacter>(EventInstigator->GetPawn()));
		for (auto& Turret : Turrets)
			Cast<ABaseCharacter>(Turret)->GetComponent<UAiComponent>(Component::AI)->AddLowpriorityCharacters(Cast<ABaseCharacter>(EventInstigator->GetPawn()));
	}

	return Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);
}

void ASoldier::CreateDrone_Implementation()
{
	if (auto Item = GetComponent<UItemComponent>(Component::Item))
	{
		if (!Item->CheckItemTypeCount(ItemType::Iron, 10)) return;

		FActorSpawnParameters spawnParameters;
		spawnParameters.Instigator = GetInstigator();
		spawnParameters.Owner = this;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Drone->Owner.GetName());
		Drones.Emplace(GetWorld()->SpawnActor<ADrone>(GetMesh()->GetBoneLocation("upperarm_r") + GetActorUpVector() * 30, GetBaseAimRotation(), spawnParameters));

		Item->RemoveItemTypeCount(ItemType::Iron, 10);
	}
}

void ASoldier::CreateTurret_Implementation()
{
	if (auto Item = GetComponent<UItemComponent>(Component::Item))
	{
		constexpr uint8 RequiredCount = 5;

		if (!Item->CheckItemTypeCount(ItemType::Iron, RequiredCount)) return;

		ULookComponent* Look = GetComponent<ULookComponent>(Component::Look);

		FCollisionQueryParams Params(NAME_None, false, this);
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActors(Drones);
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Look->GetCamera()->GetComponentLocation(), Look->GetCamera()->GetComponentLocation() + GetBaseAimRotation().Vector() * 500.f, ECollisionChannel::ECC_Visibility, Params))
		{
			if (HitResult.GetActor()->IsA(ACharacter::StaticClass())) return;

			if (GetComponent<USkillComponent>(Component::Skill)->UseSkill(0))
			{

				FActorSpawnParameters spawnParameters;
				spawnParameters.Instigator = GetInstigator();
				spawnParameters.Owner = this;
				GetWorld()->SpawnActor<AGroundTurret>(HitResult.ImpactPoint + FVector(0., 0., 100.), GetActorRotation(), spawnParameters);

				Item->RemoveItemTypeCount(ItemType::Iron, RequiredCount);

			}
		}
	}
}

void ASoldier::TargetOutline()
{
	auto CameraLocation = GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentLocation();

	UAIControllComponent* Controll = GetComponent<UAIControllComponent>(Component::AIControll);

	FCollisionQueryParams Params(NAME_None, false, this);
	Params.AddIgnoredActor(this);
	Params.bIgnoreTouches = true;
	FHitResult Hit;

	if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + GetBaseAimRotation().Vector() * 1000.f, ECollisionChannel::ECC_Visibility, Params))
	{
		if (auto Character = Cast<ACharacter>(Hit.GetActor()))
		{
			if (Character->Owner != this )
			{
				if (LookCharacter != Character)
				{
					if (LookCharacter && LookCharacter != Controll->GetTarget() && LookCharacter->GetMesh()->CustomDepthStencilValue & 2)
					{
						LookCharacter->GetMesh()->SetCustomDepthStencilValue(LookCharacter->GetMesh()->CustomDepthStencilValue - 2);
					}
					LookCharacter = Character;

					if (!(LookCharacter->GetMesh()->CustomDepthStencilValue & 2))
					{
						LookCharacter->GetMesh()->SetCustomDepthStencilValue(LookCharacter->GetMesh()->CustomDepthStencilValue + 2);
					}
				}
				return;
			}
		}
	}
	
	if (LookCharacter)
	{
		if (LookCharacter != Controll->GetTarget() && LookCharacter->GetMesh()->CustomDepthStencilValue & 2)
		{
			LookCharacter->GetMesh()->SetCustomDepthStencilValue(LookCharacter->GetMesh()->CustomDepthStencilValue - 2);
		}
		LookCharacter = nullptr;
	}
	
}