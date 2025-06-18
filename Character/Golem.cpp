// Fill out your copyright notice in the Description page of Project Settings.


#include "Golem.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
AGolem::AGolem()
{
	FootAudio->DestroyComponent();

	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	GetMesh()->DestroyComponent();
	GetCapsuleComponent()->SetCapsuleRadius(8.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(8.f);
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NC(TEXT("NiagaraSystem'/Game/EnergySourceFX/FX/NS_EnergyCore_4.NS_EnergyCore_4'"));

	if (NC.Succeeded())
	{
		Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
		Niagara->SetAsset(NC.Object);
		Niagara->SetFloatParameter(TEXT("Lifetime"), 0.01f);
		Niagara->SetFloatParameter(TEXT("Radius"), 1.f);
		Niagara->SetFloatParameter(TEXT("Size"), 20.f);
		Niagara->SetFloatParameter(TEXT("Spawn Probability"), 1.f);
		Niagara->SetFloatParameter(TEXT("SpawnRate"), 10000000.f);
		Niagara->SetFloatParameter(TEXT("Velocity"), 0.f);
		Niagara->SetupAttachment(GetCapsuleComponent());
		
	}

	FlightHeight = 100;
}

void AGolem::BeginPlay()
{
	Super::Super::BeginPlay();
}

void AGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Fly(DeltaTime);
	
}
void AGolem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AGolem::Fall);
}

void AGolem::Fly(float DeltaSeconds)
{
	FCollisionQueryParams Params(NAME_None, false, this);
	Params.AddIgnoredComponent(GetCapsuleComponent());
	Params.bIgnoreTouches = true;
	FHitResult Hit;
	bool Result = GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + GetActorUpVector() * -10000.f, ECollisionChannel::ECC_EngineTraceChannel2, Params);
	if (Result)
	{
		ACharacter* Character = Cast<ACharacter>(Hit.GetActor());
		if (Cast<UShapeComponent>(Hit.Component))
		{
			Params.AddIgnoredComponent(Hit.Component.Get());
		}
		else
		{
			if (Hit.Distance > FlightHeight || Hit.Distance < FlightHeight)
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, FMath::FInterpTo(GetActorLocation().Z, Hit.ImpactPoint.Z + FlightHeight, DeltaSeconds, 1)), true);
			}
			return;
		}


		TArray<FHitResult> HitResults;
		Result = GetWorld()->LineTraceMultiByChannel(HitResults, GetActorLocation(), GetActorLocation() + GetActorUpVector() * -10000.f, ECollisionChannel::ECC_EngineTraceChannel2, Params);
		if (Result)
		{
			for (FHitResult HitResult : HitResults)
			{
				Character = Cast<ACharacter>(HitResult.GetActor());
				if (Cast<UShapeComponent>(HitResult.Component))
				{

				}
				else
				{
					if (HitResult.Distance > FlightHeight || HitResult.Distance < FlightHeight)
					{
						SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, FMath::FInterpTo(GetActorLocation().Z, HitResult.ImpactPoint.Z + FlightHeight, DeltaSeconds, 1)), true);
					}
					return;
				}
			}
		}
	}
}

void AGolem::Jump()
{
	FlightHeight = FMath::Clamp(FlightHeight + 10., 0., 160.);
}

void AGolem::Fall()
{
	FlightHeight = FMath::Clamp(FlightHeight - 10., 0., 160.);
}
