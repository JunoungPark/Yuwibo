// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Controller/PlayerControllAIController.h"
#include "Yuwibo/Actor/Projectile/Bullet.h"
#include "Yuwibo/Actor/Projectile/ProjectilePool.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"	
#include "NiagaraSystem.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Yuwibo/Animinstance/DroneAnimInstance.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Yuwibo/Component/MapComponent.h"
#include "Yuwibo/Component/ItemComponent.h"
ADrone::ADrone()
{
	CharacterName = TEXT("Drone");

	SetComponent<UAiComponent>(Component::AI);

	GetComponent<UMoveComponent>(Component::Move)->SetAttackCanMove();
	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADrone::OverlapCharacter);
	//SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADrone::OverlapEndCharacter);


	//AIControllerClass = APlayerControllAIController::StaticClass();
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	ProjectilePool = CreateDefaultSubobject<UProjectilePool>(FName("ProjectilePool"));

	GetMesh()->SetRelativeScale3D(FVector(0.25));

	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;

	GetCapsuleComponent()->SetCapsuleHalfHeight(20.2f);
	GetCapsuleComponent()->SetCapsuleRadius(20.2f);

	ConstructorHelpers::FClassFinder<UDroneAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Drone/DroneAB.DroneAB_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Fighter_Drone/mesh/fighter_drone_mesh.fighter_drone_mesh'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);

		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -39.5));
	}
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS(TEXT("NiagaraSystem'/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_AR_Muzzle_flashes_01.NS_AR_Muzzle_flashes_01'"));
	if (NS.Succeeded())
	{
		ConstructorHelpers::FObjectFinder<UNiagaraSystem> SNS(TEXT("NiagaraSystem'/Game/Gun_VFX/Niagara_FX/Misc_Gun_FX/NS_AR_Over_heating_smoke.NS_AR_Over_heating_smoke'"));

		if (SNS.Succeeded()) 
		{
			auto LeftTopMuzzle = CreateDefaultSubobject<UNiagaraComponent>("LeftTopMuzzle");
			LeftTopMuzzle->SetAsset(NS.Object);
			LeftTopMuzzle->SetupAttachment(GetMesh(), TEXT("b_gun_top_left_end"));

			auto SecondLeftTopMuzzle = CreateDefaultSubobject<UNiagaraComponent>("SecondLeftTopMuzzle");
			SecondLeftTopMuzzle->SetAsset(SNS.Object);
			SecondLeftTopMuzzle->SetupAttachment(GetMesh(), TEXT("b_gun_top_left_end"));

			TopMuzzles.Emplace(LeftTopMuzzle);
			TopMuzzles.Emplace(SecondLeftTopMuzzle);

			auto LeftBotMuzzle = CreateDefaultSubobject<UNiagaraComponent>("LeftBotMuzzle");
			LeftBotMuzzle->SetAsset(NS.Object);
			LeftBotMuzzle->SetupAttachment(GetMesh(), TEXT("b_gun_bot_left_end"));

			auto SecondLeftBotMuzzle = CreateDefaultSubobject<UNiagaraComponent>("SecondLeftBotMuzzle");
			SecondLeftBotMuzzle->SetAsset(SNS.Object);
			SecondLeftBotMuzzle->SetupAttachment(GetMesh(), TEXT("b_gun_bot_left_end"));

			BotMuzzles.Emplace(LeftBotMuzzle);
			BotMuzzles.Emplace(SecondLeftBotMuzzle);

			auto RightTopMuzzle = CreateDefaultSubobject<UNiagaraComponent>("RightTopMuzzle");
			RightTopMuzzle->SetAsset(NS.Object);
			RightTopMuzzle->SetupAttachment(GetMesh(), TEXT("b_gun_top_right_end"));

			auto SecondRightTopMuzzle = CreateDefaultSubobject<UNiagaraComponent>("SecondRightTopMuzzle");
			SecondRightTopMuzzle->SetAsset(SNS.Object);
			SecondRightTopMuzzle->SetupAttachment(GetMesh(), TEXT("b_gun_top_right_end"));

			TopMuzzles.Emplace(RightTopMuzzle);
			TopMuzzles.Emplace(SecondRightTopMuzzle);

			auto RightBotMuzzle = CreateDefaultSubobject<UNiagaraComponent>("RightBotMuzzle");
			RightBotMuzzle->SetAsset(NS.Object);
			RightBotMuzzle->SetupAttachment(GetMesh(), TEXT("b_gun_bot_right_end"));

			auto SecondRightBotMuzzle = CreateDefaultSubobject<UNiagaraComponent>("SecondRightBotMuzzle");
			SecondRightBotMuzzle->SetAsset(SNS.Object);
			SecondRightBotMuzzle->SetupAttachment(GetMesh(), TEXT("b_gun_bot_right_end"));

			BotMuzzles.Emplace(RightBotMuzzle);
			BotMuzzles.Emplace(SecondRightBotMuzzle);

			for (auto& Muzzle : TopMuzzles)
			{
				Muzzle->SetAutoActivate(false);
			}
			for (auto& Muzzle : BotMuzzles)
			{
				Muzzle->SetAutoActivate(false);
			}
		}
		
	}

	

	ULookComponent* Look = GetComponent<ULookComponent>(Component::Look);

	Look->GetSpringArm()->DestroyComponent();
	Look->GetCamera()->bUsePawnControlRotation = true;
	Look->GetCamera()->SetupAttachment(GetMesh(), TEXT("bodySocket"));


	//if (HpWidget)
	//{
	//	HpWidget->RemoveFromViewport();
	//}
}

//void ADrone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ADrone, FlightHeight);
//	DOREPLIFETIME(ADrone, UpDown);
//}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
		ProjectilePool->InitializePool<ABullet>(this, 0.12);
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ADrone::Fall);

	UMoveComponent* Move = GetComponent<UMoveComponent>(Component::Move);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, Move, &UMoveComponent::SetAttack<false>);

	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &ADrone::Fall);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &ADrone::Jump);
	//PlayerInputComponent->BindAction(TEXT("VisibleItemWindow"), EInputEvent::IE_Pressed, this, &ADrone::VisibleItemWindow);
	//PlayerInputComponent->BindAction(TEXT("OpenItemBox"), EInputEvent::IE_Pressed, this, &ADrone::OpenItemBox);
}

AProjectile* ADrone::GetBullet(FVector Location, FRotator Rotation)
{
	return ProjectilePool->GetProjectile<ABullet>(Location, Rotation);
}

void ADrone::ResetTopFx()
{
	for (auto& Muzzle : TopMuzzles)
	{
		Muzzle->ResetSystem();
	}
}

void ADrone::ResetBotFx()
{
	for (auto& Muzzle : TopMuzzles)
	{
		Muzzle->ResetSystem();
	}
}

//void ADrone::Fly(float DeltaSeconds)
//{
//	if (!IsPlayerControlled()) return;
//	
//	if (UpDown)
//	{
//		FlightHeight =  FMath::Clamp(FlightHeight + (UpDown > 0 ? 10. : -10), 0., 300.);
//	}
//	 
//	FCollisionQueryParams Params(NAME_None, false, this);
//	Params.AddIgnoredActor(this);
//	Params.bIgnoreTouches = true;
//	
//	TArray<FHitResult> HitResults; 
//	if (GetWorld()->LineTraceMultiByChannel(HitResults, GetActorLocation(), GetActorLocation() + FVector(0, 0, -1) * 10000.f, ECollisionChannel::ECC_Visibility, Params))
//	{
//		for (FHitResult& HitResult : HitResults)
//		{
//
//			if (HitResult.Distance != FlightHeight)
//			{
//				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, FMath::FInterpTo(GetActorLocation().Z, HitResult.ImpactPoint.Z + FlightHeight, DeltaSeconds, 7)), true);
//
//
//			}
//			return;
//		}
//		
//	}
//}

//void ADrone::SetFlightHeight_Implementation(bool Up)
//{
//}

void ADrone::Call_Implementation(float value)
{
	UE_LOG(LogTemp, Log, TEXT("%f"), value);
}

void ADrone::Jump()
{
	GetComponent<UDroneAnimComponent>(Component::Anim)->SetUpDown(1);
}

void ADrone::Fall()
{
	GetComponent<UDroneAnimComponent>(Component::Anim)->SetUpDown(-1);
}

//void ADrone::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	if (!HasAuthority() && !IsLocallyControlled()) return;
//
//	Fly(DeltaTime);
//
//}
//
float ADrone::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(!IsPlayerControlled()) return 0.0f;

	return Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);
}
