// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTurret.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Actor/Projectile/ProjectilePool.h"
#include "Yuwibo/Animinstance/GroundTurretAnimInstance.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Actor/Projectile/Bullet.h"
#include "Soldier.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

//void AGroundTurret::OnHealthUpdate()
//{
//	if (CurrentHealth <= 0)
//	{
//		Cast<UGroundTurretAnimInstance>(GetMesh()->GetAnimInstance())->SetIsDead(true);
//
//		FTimerHandle DestoyTimerHandle;
//		GetWorldTimerManager().SetTimer(DestoyTimerHandle, this, &AGroundTurret::Disappear, 3.0f);
//
//	}
//
//	UE_LOG(LogTemp, Log, TEXT("%f"), CurrentHealth);
//	//HpWidget->UpdateHp();
//}
//void AGroundTurret::FindTarget()
//{
//	if (!HasAuthority() && !IsLocallyControlled()) return;
//
//	if (GetController() && GetController()->IsPlayerController()) 
//	{
//		FCollisionQueryParams Params(NAME_None, false, this);
//		Params.bIgnoreTouches = true;
//		Params.AddIgnoredActor(this);
//		
//		FHitResult HitResult;
//
//		auto ComponentLocation = GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentLocation();
//
//		auto Result = GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + GetControlRotation().Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params);
//		
//		GunRotation = Result ? UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.ImpactPoint) - FRotator(0., GetMesh()->GetComponentRotation().Yaw, 0.) : GetControlRotation() - FRotator(0., GetMesh()->GetComponentRotation().Yaw, 0.);
//		
//	}
//	else if (auto Target = GetComponent<UAiComponent>(Component::AI)->GetTarget())
//	{
//		GunRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()) - FRotator(0., GetMesh()->GetComponentRotation().Yaw, 0.);
//		GetComponent<UMoveComponent>(Component::Move)->SetAttack<true>();
//	}
//	else 
//		GetComponent<UMoveComponent>(Component::Move)->SetAttack<false>();
//
//	UE_LOG(LogTemp, Log, TEXT("%f,%f,%f"), GunRotation.Pitch, GunRotation.Yaw, GunRotation.Roll);
//	
//}

// Sets default values
AGroundTurret::AGroundTurret()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	SetComponent<UAiComponent>(Component::AI);
	SetComponent<UFourFootIkComponent>(Component::FootIk)->BindSocket("Bone016", "Bone012", "Bone004", "Bone008");

	ProjectilePool = CreateDefaultSubobject<UProjectilePool>(FName("ProjectilePool"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/acs17/Meshes/Mech_ASC17.Mech_ASC17'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocation(FVector(0., 0., -86.));
	}

	ConstructorHelpers::FClassFinder<UGroundTurretAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Turret/GroundTurret/GroundTurretAB.GroundTurretAB_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> MI(TEXT("Material'/Game/acs17/Textures/T_ACS17_M3_BaseColor_Mat.T_ACS17_M3_BaseColor_Mat'")); 
	if (MI.Succeeded())
	{
		GetMesh()->SetMaterial(0, MI.Object);
	}

	GetMesh()->SetRelativeScale3D(FVector(0.5));
}

//void AGroundTurret::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AGroundTurret, GunRotation);
//}

// Called when the game starts or when spawned

void AGroundTurret::BeginPlay()
{
	Super::BeginPlay();

	GetComponent<UFourFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction(GetMesh()->GetAnimInstance(), FName("FootIk"));

	ProjectilePool->InitializePool<ABullet>(this, 1.9);
}

AProjectile* AGroundTurret::GetBullet(FVector Location, FRotator Rotation)
{
	return ProjectilePool->GetProjectile<ABullet>(Location, Rotation);
}

// Called every frame
//void AGroundTurret::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	FindTarget();
//}

// Called to bind functionality to input
void AGroundTurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, GetComponent<UMoveComponent>(Component::Move), &UMoveComponent::SetAttack<true>);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, GetComponent<UMoveComponent>(Component::Move), &UMoveComponent::SetAttack<false>);
}

float AGroundTurret::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsPlayerControlled() && HasAuthority()) GetComponent<UAiComponent>(Component::AI)->AddLowpriorityCharacters(Cast<ABaseCharacter>(EventInstigator->GetPawn()));

	return Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);

}

