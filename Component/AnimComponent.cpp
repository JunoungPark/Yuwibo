// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimComponent.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Yuwibo/Character/GroundTurret.h"
#include "Yuwibo/Animinstance/GroundTurretAnimInstance.h"
#include "Yuwibo/Animinstance/BaseEnemyAnimInstance.h"
#include "MoveComponent.h"
#include "Yuwibo/Character/Trainer.h"
#include "Yuwibo/Animinstance/SoldierAnimInstance.h"
#include "Yuwibo/Character/TankLv3.h"
#include "Yuwibo/Animinstance/TankAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "LookComponent.h"
#include "AiComponent.h"
#include "Camera/CameraComponent.h"
#include "Yuwibo/Character/Golem.h"
#include "Camera/CameraActor.h"
#include "Yuwibo/Character/Moose.h"
#include "Yuwibo/Animinstance/MooseAnimInstance.h"
#include "Yuwibo/Animinstance/TrainerAnimInstance.h"
#include "Yuwibo/Animinstance/BodyChangerAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"
#include "Yuwibo/Character/Soldier.h"
#include "Yuwibo/Animinstance/DroneAnimInstance.h"
// Sets default values for this component's properties
UAnimComponent::UAnimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	// ...
}

void UAnimComponent::SetIsCrouch_Implementation(bool IsTrue)
{
	IsCrouch = IsTrue;
}

void UAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAnimComponent, SyncTrigger);

	DOREPLIFETIME(UAnimComponent, DefaultRotator);

	DOREPLIFETIME(UAnimComponent, IsAI);

	DOREPLIFETIME(UAnimComponent, IsAttack);

	DOREPLIFETIME(UAnimComponent, IsCrouch);

	DOREPLIFETIME(UAnimComponent, IsFalling);

	DOREPLIFETIME(UAnimComponent, Velocity);

	DOREPLIFETIME(UAnimComponent, GroundSpeed);

	DOREPLIFETIME(UAnimComponent, IsMoving);

	DOREPLIFETIME(UAnimComponent, AimRotation);

	DOREPLIFETIME(UAnimComponent, BlendAlpha);

	DOREPLIFETIME(UAnimComponent, YawOffset);

	DOREPLIFETIME(UAnimComponent, RootYaw);

	//DOREPLIFETIME(UAnimComponent, AttackYaw);

	DOREPLIFETIME(UAnimComponent, MovingRotation);

	//DOREPLIFETIME(UAnimComponent, FinalRotation);

	DOREPLIFETIME(UAnimComponent, ForwardValue);

	DOREPLIFETIME(UAnimComponent, RightValue);

	DOREPLIFETIME(UAnimComponent, IsDead);


}

// Called when the game starts
void UAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	//if (!Character->HasAuthority())	SetComponentTickEnabled(false);

	if (Character->HasAuthority()) DefaultRotator = Character->GetMesh()->GetRelativeRotation();

	if (auto MyGameInstance = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		RotationToMove = MyGameInstance->GetCharacterData(Character->GetCharacterName())->RotationToMovement;
	}

	if (Character)
		Anim = Cast<UBaseAnimInstance>(Character->GetMesh()->GetAnimInstance());

}

void UAnimComponent::SetRootYaw(float DeltaSeconds)
{
	if (!Anim) return;

	if (IsStop)
	{
		if (Anim->Montage_IsPlaying(Anim->GetAttackMontage()) || IsMoving)
		{
		
			RootYaw = MovingRotation.Yaw + RootYaw - Character->GetActorRotation().Yaw;

			while (RootYaw > 180.f)
			{
				RootYaw -= 360.f;
			}
			while (RootYaw < -180.f)
			{
				RootYaw += 360.f;
			}

			FinalRotation = MovingRotation;
			MovingRotation = Character->GetActorRotation();

			IsStop = false;
		}
	}

	if (Attacking)
	{
		if (!Anim->Montage_IsPlaying(Anim->GetAttackMontage()) && IsMoving)
		{
			RootYaw = MovingRotation.Yaw + RootYaw - Character->GetActorRotation().Yaw;

			while (RootYaw > 180.f)
			{
				RootYaw -= 360.f;
			}
			while (RootYaw < -180.f)
			{
				RootYaw += 360.f;
			}

			FinalRotation = MovingRotation;
			MovingRotation = Character->GetActorRotation(); 
			
			Attacking = false;
		}
	}

	if (Anim->Montage_IsPlaying(Anim->GetAttackMontage()))
	{
		if(!Attacking) Attacking = true;

		RootYaw = FMath::FInterpTo(RootYaw, AttackYaw, DeltaSeconds, 20);
		
		//FinalRotation = MovingRotation;
		//MovingRotation = Character->GetActorRotation();
		
		Character->GetMesh()->SetWorldRotation(MovingRotation + DefaultRotator + FRotator(0., RootYaw, 0.));

	}
	else if (IsMoving)
	{
		if (!RotationToMove || IsAI)
		{
			while (RootYaw > 180.f)
			{
				RootYaw -= 360.f;
			}
			while (RootYaw < -180.f)
			{
				RootYaw += 360.f;
			}
			RootYaw = FMath::FInterpTo(RootYaw, 0, DeltaSeconds, 10);

		}
		else
		{
			while (RootYaw > YawOffset + 180.f)
			{
				YawOffset += 360.f;
			}
			while (RootYaw < YawOffset - 180.f)
			{
				YawOffset -= 360.f;
			}
			RootYaw = FMath::FInterpTo(RootYaw, YawOffset, DeltaSeconds, 10);

		}

		//if (Character->GetCharacterName() == "Vampire")
		//{
		//	UE_LOG(LogTemp, Log, TEXT("%f %f"), RootYaw, YawOffset);
		//}
		MovingRotation = Character->GetActorRotation();
		FinalRotation = MovingRotation;

		Character->GetMesh()->SetRelativeRotation(DefaultRotator + FRotator(0., RootYaw, 0.));
	}
	else
	{
		if (!IsStop) IsStop = true;

		Character->GetMesh()->SetWorldRotation(MovingRotation + DefaultRotator + FRotator(0., RootYaw, 0.));

	}
	
}

void UAnimComponent::SetIsAI()
{
	IsAI = Character->GetController() && !Character->GetController()->IsPlayerController();
}

void UAnimComponent::UpdateAnimation(float DeltaSeconds)
{
	if (!CharacterMovement || !Anim) return;

	IsFalling = CharacterMovement->IsFalling();

	Velocity = CharacterMovement->Velocity;
	//UE_LOG(LogTemp, Log, TEXT("%f, %f, %f"), Velocity.X, Velocity.Y, Velocity.Z);

	GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

	if (GroundSpeed > 0.f)
		IsMoving = true;
	else
		IsMoving = false;

	AimRotation = Character->GetBaseAimRotation();

	BlendAlpha = FMath::FInterpTo(BlendAlpha, IsMoving ? GroundSpeed / CharacterMovement->GetMaxSpeed() : 0., DeltaSeconds, 20);

	YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(Velocity), AimRotation).Yaw;
	
	SetIsAI();

	SetRootYaw(DeltaSeconds);
}

// Called every frame
void UAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character->HasAuthority())
	{
		UpdateAnimation(DeltaTime);
		// ...
		OnRep_UpdateAnimationValues();

		SyncTrigger++;
	}
	else if (Character->IsLocallyControlled() || Character->IsA(ATank::StaticClass())) SetRootYaw(DeltaTime);
	else
	{
		if(Anim && !Anim->Montage_IsPlaying(Anim->GetAttackMontage()) && IsMoving) Character->GetMesh()->SetRelativeRotation(DefaultRotator + FRotator(0., RootYaw, 0.));
		else Character->GetMesh()->SetWorldRotation(MovingRotation + DefaultRotator + FRotator(0., RootYaw, 0.));
	}
	
}

void UAnimComponent::OnRep_UpdateAnimationValues()
{
	//Anim->IsAttack = IsAttack;

	if (!Anim) return;
	
	Anim->IsAttack = IsAttack;

	Anim->IsCrouch = IsCrouch;

	Anim->IsFalling = IsFalling;

	Anim->Velocity = Velocity;

	Anim->GroundSpeed = GroundSpeed;

	Anim->IsMoving = IsMoving;

	Anim->AimRotation = AimRotation;

	Anim->BlendAlpha = BlendAlpha;

	Anim->YawOffset = YawOffset;

	//Anim->RootYaw = RootYaw;

	//Anim->AttackYaw = AttackYaw;

	Anim->ForwardValue = ForwardValue;

	Anim->RightValue = RightValue;

	Anim->IsDead = IsDead;


}

void UAnimComponent::Bind(ABaseCharacter* NewCharacter)
{
	Character = NewCharacter;

	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();

	}


}

void UBodyChangerAnimComponent::SetAttackRotation()
{

	auto ComponentLocation = Character->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentLocation();

	FCollisionQueryParams Params(NAME_None, false, Character);
	Params.bIgnoreTouches = true;
	FHitResult HitResult;

	auto Result = GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + Character->GetControlRotation().Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params);

	if (Result)
	{
		AttackHitResult = HitResult.ImpactPoint;
		//DrawDebugLine(GetWorld(), Character->GetCamera()->GetComponentLocation(), AttackHitResult, FColor::Green, false, 2.f);

		LookSky = false;

		AttackYaw = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), AttackHitResult).Yaw - MovingRotation.Yaw;
		
		if (Anim->Montage_GetCurrentSection(Anim->GetAttackMontage()) == FName("Attack_0"))
		{
			AttackYaw = 0.f;

			return;
		}

		while (RootYaw > AttackYaw + 180.f)
		{
			AttackYaw += 360.f;
		}
		while (RootYaw < AttackYaw - 180.f)
		{
			AttackYaw -= 360.f;
		}
		
	}
	else
	{
		LookSky = true;

		if (Anim->Montage_GetCurrentSection(Anim->GetAttackMontage()) == FName("Attack_0"))
		{
			AttackYaw = 0.f;

			return;
		}

		AttackYaw = AimRotation.Yaw - MovingRotation.Yaw;

		while (RootYaw > AttackYaw + 180.f)
		{
			AttackYaw += 360.f;
		}
		while (RootYaw < AttackYaw - 180.f)
		{
			AttackYaw -= 360.f;
		}
	}

	BodyChangerAttackYaw = RootYaw;

	SetServerAttackYaw(AttackYaw);
	
	OnRep_UpdateAnimationValues();
}

void UBodyChangerAnimComponent::SetRootYaw(float DeltaSeconds)
{
	if (!Anim) return;

	if (IsStop)
	{
		if (Anim->Montage_IsPlaying(Anim->GetAttackMontage()) || IsMoving)
		{

			RootYaw = MovingRotation.Yaw + RootYaw - Character->GetActorRotation().Yaw;

			while (RootYaw > 180.f)
			{
				RootYaw -= 360.f;
			}
			while (RootYaw < -180.f)
			{
				RootYaw += 360.f;
			}

			FinalRotation = MovingRotation;
			MovingRotation = Character->GetActorRotation();

			IsStop = false;
		}
	}

	if (Attacking)
	{
		if (!Anim->Montage_IsPlaying(Anim->GetAttackMontage()) && IsMoving)
		{
			RootYaw = MovingRotation.Yaw + RootYaw - Character->GetActorRotation().Yaw;

			while (RootYaw > 180.f)
			{
				RootYaw -= 360.f;
			}
			while (RootYaw < -180.f)
			{
				RootYaw += 360.f;
			}

			FinalRotation = MovingRotation;
			MovingRotation = Character->GetActorRotation();

			Attacking = false;
		}
	}

	if (Anim->Montage_IsPlaying(Anim->GetAttackMontage()))
	{
		if (!Attacking) Attacking = true;

		BodyChangerAttackYaw = FMath::FInterpTo(BodyChangerAttackYaw, AttackYaw, DeltaSeconds, 20);

		RootYaw = BodyChangerAttackYaw;
		//FinalRotation = MovingRotation;
		//MovingRotation = Character->GetActorRotation();

		Character->GetMesh()->SetWorldRotation(MovingRotation + DefaultRotator + FRotator(0., BodyChangerAttackYaw, 0.));

	}
	else if (IsMoving)
	{
		if (!RotationToMove || IsAI)
		{
			while (RootYaw > 180.f)
			{
				RootYaw -= 360.f;
			}
			while (RootYaw < -180.f)
			{
				RootYaw += 360.f;
			}
			RootYaw = FMath::FInterpTo(RootYaw, 0, DeltaSeconds, 10);

		}
		else
		{
			while (RootYaw > YawOffset + 180.f)
			{
				YawOffset += 360.f;
			}
			while (RootYaw < YawOffset - 180.f)
			{
				YawOffset -= 360.f;
			}
			RootYaw = FMath::FInterpTo(RootYaw, YawOffset, DeltaSeconds, 10);

		}

		//if (Character->GetCharacterName() == "Vampire")
		//{
		//	UE_LOG(LogTemp, Log, TEXT("%f %f"), RootYaw, YawOffset);
		//}
		MovingRotation = Character->GetActorRotation();
		FinalRotation = MovingRotation;

		Character->GetMesh()->SetRelativeRotation(DefaultRotator + FRotator(0., RootYaw, 0.));
	}
	else
	{
		if (!IsStop) IsStop = true;

		Character->GetMesh()->SetWorldRotation(MovingRotation + DefaultRotator + FRotator(0., RootYaw, 0.));

	}
}

void UBodyChangerAnimComponent::SetServerAttackYaw_Implementation(float Yaw)
{
	SetNetAttackYaw(Yaw);
}

void UBodyChangerAnimComponent::SetNetAttackYaw_Implementation(float Yaw)
{
	if (Character->IsLocallyControlled()) return;

	AttackYaw = Yaw;
}

void UBodyChangerAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Anim)
		BodyChangerAnim = Cast<UBodyChangerAnimInstance>(Anim);

}

void UBodyChangerAnimComponent::OnRep_UpdateAnimationValues()
{
	Super::OnRep_UpdateAnimationValues();

	if (!BodyChangerAnim) return;

	BodyChangerAnim->AttackHitResult = AttackHitResult;

	BodyChangerAnim->LookSky = LookSky;
}

void UDroneAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	UpDown = 0;

	if (Anim)
		DroneAnim = Cast<UDroneAnimInstance>(Anim);
}

void UDroneAnimComponent::SetRootYaw(float DeltaSeconds)
{
	if (Character->IsPlayerControlled())
	{
		auto ComponentLocation = Character->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentLocation();

		FCollisionQueryParams Params(NAME_None, false, Character);
		Params.bIgnoreTouches = true;
		Params.AddIgnoredActor(Character);

		FHitResult HitResult;
		LookSky = !GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + Character->GetControlRotation().Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params);

		TargetLocation = HitResult.ImpactPoint;

		auto Direct = (TargetLocation - Character->GetActorLocation()).GetSafeNormal();
		FRotator LookAtRotation = FRotator(FMath::RadiansToDegrees(FMath::Atan2(Direct.Z, FVector(Direct.X, Direct.Y, 0).Size())), Character->GetControlRotation().Yaw, 0.0f);

		Character->SetActorRotation(!LookSky ? LookAtRotation : Character->GetControlRotation());
	}
	else if (Character->HasAuthority()) AIMove(DeltaSeconds);
}

void UDroneAnimComponent::AIMove(float DeltaSeconds)
{
	if (Character->IsPlayerControlled()) return;

	if (auto Soldier = Cast<ASoldier>(Character->Owner))
	{
		if(auto AI = Character->GetComponent<UAiComponent>(Component::AI))
		{
			auto Target = AI->GetTarget();
			auto LockOn = AI->GetCheckOwnerLockOnAndTarget();

			auto CharacterLocation = Character->GetActorLocation();
			
			auto Direction = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, ((LockOn && !(Character->GetDistanceTo(Soldier) > AI->GetSphereRadius())) ? Target->GetTargetLocation() : Soldier->GetTargetLocation()) + FVector(0., 0., 200.)).Vector();

			if ((FVector::Dist(CharacterLocation + Direction * 600 * DeltaSeconds, Soldier->GetActorLocation()) <= AI->GetSphereRadius() && LockOn && Character->GetDistanceTo(Target) > 500.)
				||
				(!LockOn && Character->GetDistanceTo(Soldier) > 500.))
			{
				Character->SetActorLocation(CharacterLocation + Direction * 600 * DeltaSeconds);
			}
			CharacterLocation = Character->GetActorLocation();

			Character->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CharacterLocation, Target ? Target->GetActorLocation() : Soldier->GetActorLocation()));

			if (Target) Character->GetComponent<UMoveComponent>(Component::Move)->SetAttack<true>();
			else Character->GetComponent<UMoveComponent>(Component::Move)->SetAttack<false>();

			FCollisionQueryParams Params(NAME_None, false, Character);
			Params.AddIgnoredComponent(Character->GetCapsuleComponent());
			Params.bIgnoreTouches = true;
			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, CharacterLocation, CharacterLocation + Direction * 600.f, ECollisionChannel::ECC_Visibility, Params))
			{
				if (Hit.GetActor() != Character->Owner)
				{
					FNavLocation ResultLocation;
					// ������̼� �ý��� ��������
					UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
					if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(Soldier->GetTargetLocation(), 300., ResultLocation))
					{
						//if (auto AIController = Cast<AAIController>(GetController()))
						//{
						//	AIController->MoveToLocation(ResultLocation.Location);
						//}
						//else
						{
							// AIController�� ������ ���� ��ġ�� ����
							Character->SetActorLocation(ResultLocation.Location);
						}
					}

				}
			}
		}
	}
}

void UDroneAnimComponent::Fly(float DeltaSeconds)
{
	if (!Character->IsPlayerControlled()) return;

	if(Character->HasAuthority())
	{
		if (UpDown)
		{
			FlightHeight = FMath::Clamp(FlightHeight + (UpDown > 0 ? 1.f : -1.f) * 300.f * DeltaSeconds, 0., 300.);
		}
	}

	FCollisionQueryParams Params(NAME_None, false, Character);
	Params.AddIgnoredActor(Character);
	Params.bIgnoreTouches = true;

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Character->GetActorLocation(), Character->GetActorLocation() + FVector(0, 0, -1) * 10000.f, ECollisionChannel::ECC_Visibility, Params))
	{
		if (HitResult.Distance != FlightHeight)
		{
			Character->SetActorLocation(FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, FMath::FInterpTo(Character->GetActorLocation().Z, HitResult.ImpactPoint.Z + FlightHeight, DeltaSeconds, 20.f)), true);
		}
	}
}

void UDroneAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UDroneAnimComponent, LookSky);

	DOREPLIFETIME(UDroneAnimComponent, TargetLocation);

	DOREPLIFETIME(UDroneAnimComponent, UpDown);

	DOREPLIFETIME(UDroneAnimComponent, FlightHeight);
}

void UDroneAnimComponent::OnRep_UpdateAnimationValues()
{
	Super::OnRep_UpdateAnimationValues();

	if (!DroneAnim) return;

	DroneAnim->LookSky = LookSky;

	DroneAnim->TargetLocation = TargetLocation;
}

void UDroneAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character->HasAuthority())
	{
		UpdateAnimation(DeltaTime);
		// ...
		OnRep_UpdateAnimationValues();

		Fly(DeltaTime);

		SyncTrigger++;
	}
	else if (Character->IsLocallyControlled()) 
	{
		SetRootYaw(DeltaTime);

		Fly(DeltaTime);
	}
}

void UDroneAnimComponent::SetUpDown_Implementation(int8 Direction)
{
	UpDown += Direction;
}

void UGroundTurretAnimComponent::UpdateAnimation(float DeltaSeconds)
{
	SetRootYaw(DeltaSeconds);

	if (IsAttack && BlendAlpha != 1)
	{
		BlendAlpha = FMath::FInterpTo(BlendAlpha, 1, DeltaSeconds, 10);
	}
	else if (!IsAttack && BlendAlpha != 0)
	{
		BlendAlpha = FMath::FInterpTo(BlendAlpha, 0, DeltaSeconds, 10);
	}

	if (Character->GetController() && Character->GetController()->IsPlayerController())
	{
		FCollisionQueryParams Params(NAME_None, false, Character);
		Params.bIgnoreTouches = true;
		Params.AddIgnoredActor(Character);

		FHitResult HitResult;

		auto ComponentLocation = Character->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentLocation();

		auto Result = GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + Character->GetControlRotation().Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params);

		GunRotation = Result ? UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), HitResult.ImpactPoint) - FRotator(0., Character->GetMesh()->GetComponentRotation().Yaw, 0.) : Character->GetControlRotation() - FRotator(0., Character->GetMesh()->GetComponentRotation().Yaw, 0.);

	}
	else if (auto Target = Character->GetComponent<UAiComponent>(Component::AI)->GetTarget())
	{
		GunRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), Target->GetActorLocation()) - FRotator(0., Character->GetMesh()->GetComponentRotation().Yaw, 0.);
		Character->GetComponent<UMoveComponent>(Component::Move)->SetAttack<true>();
	}
	else
		Character->GetComponent<UMoveComponent>(Component::Move)->SetAttack<false>();

}

void UGroundTurretAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGroundTurretAnimComponent, GunRotation);
}

void UGroundTurretAnimComponent::BeginPlay()
{
	Super::Super::BeginPlay();

	if (Character->HasAuthority()) DefaultRotator = Character->GetMesh()->GetRelativeRotation();

	if (Character)
		Anim = Cast<UBaseAnimInstance>(Character->GetMesh()->GetAnimInstance());
	if(Anim)
		GroundTurretAnim = Cast<UGroundTurretAnimInstance>(Anim);
}

void UGroundTurretAnimComponent::SetRootYaw(float DeltaSeconds)
{
	Character->GetMesh()->SetWorldRotation(DefaultRotator);
}

void UGroundTurretAnimComponent::OnRep_UpdateAnimationValues()
{
	Super::OnRep_UpdateAnimationValues();

	if (!GroundTurretAnim) return;

	GroundTurretAnim->GunRotation = GunRotation;
}

void USoldierAnimComponent::UpdateAnimation(float DeltaSeconds)
{
	Super::UpdateAnimation(DeltaSeconds);

	auto CharacterRotation = Character->GetActorRotation();

	HeadRotator.Pitch = AimRotation.Roll - CharacterRotation.Roll;

	HeadRotator.Yaw = 0.;

	HeadRotator.Roll = -FMath::Clamp(AimRotation.Pitch - CharacterRotation.Pitch + (AimRotation.Pitch - CharacterRotation.Pitch <= 90 ? 0 : -360), -50., 90.);

	HandRotator.Pitch = 0.;

	HandRotator.Yaw = FMath::Clamp(AimRotation.Yaw - CharacterRotation.Yaw - RootYaw + 10, -80., 100.);

	HandRotator.Roll = -FMath::Clamp(AimRotation.Pitch - CharacterRotation.Pitch + 40, -45., 95.);

	//if (IsAttack)
	//{
	//	BlendWeight = FMath::FInterpTo(BlendWeight, 1, DeltaSeconds, 10);
	//}
	//else
	//{
	//	BlendWeight = FMath::FInterpTo(BlendWeight, 0, DeltaSeconds, 10);
	//}
	
}

void USoldierAnimComponent::SetRootYaw(float DeltaSeconds)
{
	CameraFinalYaw = CameraMovingYaw;
	CameraMovingYaw = AimRotation.Yaw;
	if (!IsMoving)
	{
		if (CameraFinalYaw < CameraMovingYaw - 1.5f)
		{
			IsRightTurn = true;
			IsLeftTurn = false;
		}
		else if (CameraFinalYaw > CameraMovingYaw + 1.5f)
		{
			IsLeftTurn = true;
			IsRightTurn = false;
		}
		else
		{
			IsLeftTurn = false;
			IsRightTurn = false;
		}
	}
	else
	{
		IsLeftTurn = false;
		IsRightTurn = false;
	}
}

void USoldierAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USoldierAnimComponent, HeadRotator);

	DOREPLIFETIME(USoldierAnimComponent, HandRotator);

	DOREPLIFETIME(USoldierAnimComponent, CameraMovingYaw);

	DOREPLIFETIME(USoldierAnimComponent, CameraFinalYaw);

	DOREPLIFETIME(USoldierAnimComponent, BlendWeight);

	DOREPLIFETIME(USoldierAnimComponent, IsLeftTurn);

	DOREPLIFETIME(USoldierAnimComponent, IsRightTurn);

}

void USoldierAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Anim)
		SoldierAnim = Cast<USoldierAnimInstance>(Anim);
}

void USoldierAnimComponent::OnRep_UpdateAnimationValues()
{
	Super::OnRep_UpdateAnimationValues();

	if (!SoldierAnim) return;

	SoldierAnim->HeadRotator = HeadRotator;

	SoldierAnim->HandRotator = HandRotator;

	SoldierAnim->CameraMovingYaw = CameraMovingYaw;

	SoldierAnim->CameraFinalYaw = CameraFinalYaw;

	SoldierAnim->BlendWeight = BlendWeight;

	SoldierAnim->IsLeftTurn = IsLeftTurn;

	SoldierAnim->IsRightTurn = IsRightTurn;
}

void UTankAnimComponent::SetRootYaw(float DeltaSeconds)
{
	Mode = Tank->GetMode();
	if (Tank->GetComponent<UMoveComponent>(Component::Move)->GetCanMove())
	{
		if (Mode == true)
		{
			if (IsMoving)
			{
				if (RootYaw - 180.f > UKismetMathLibrary::MakeRotFromX(Velocity).Yaw - 90.f)
				{
					RootYaw -= 360.f;
				}
				else if (RootYaw + 180.f < UKismetMathLibrary::MakeRotFromX(Velocity).Yaw - 90.f)
				{
					RootYaw += 360.f;
				}
				RootYaw = FMath::FInterpTo(RootYaw, UKismetMathLibrary::MakeRotFromX(Velocity).Yaw - 90.f, DeltaSeconds, 10);

				if (!Tank->GetRollSound())
				{
					Tank->SetRollSound();
				}
			}
			else
			{
				if (Tank->GetRollSound())
				{
					Tank->SetRollSound();
				}
			}
		}
		else
		{

			if (IsMoving)
			{
				YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(Velocity), FRotator(0.f, RootYaw, 0.f)).Yaw - 90.f;
				if (YawOffset < -180.f)
				{
					YawOffset += 360.f;
				}
				else if (YawOffset > 180.f)
				{
					YawOffset -= 360.f;
				}
			}
		}
	}


	Tank->GetMesh()->SetWorldRotation(FRotator(0.f, RootYaw, 0.f));

	UpdateTankAnim(DeltaSeconds);

}

void UTankAnimComponent::SetAttackInfo_Implementation(bool LookSky, FVector TargetLocation, double Yaw, double Pitch)
{
	Tank->SetLookGun(Yaw, Pitch);

	SetNetAttackInfo(LookSky, TargetLocation);
}

void UTankAnimComponent::SetNetAttackInfo_Implementation(bool LookSky, FVector TargetLocation)
{
	if (Character->IsLocallyControlled()) return;

	Result = LookSky;

	AttackHitResult = TargetLocation;

}

void UTankAnimComponent::GunAttack_Implementation(bool IsTrue, bool IsLookSky, FVector Vector)
{
	TankAnim->GunAttack(IsTrue, IsLookSky, Vector);
}

void UTankAnimComponent::UpdateTankAnim(float DeltaSeconds)
{
	if (!IsValid(Tank))
		return;

	if(Character->IsLocallyControlled())
	{
		if (IsAttack || Character->GetCharacterMovement()->Velocity.SizeSquared() > 0.f)
		{
			ComponentLocation = Character->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentLocation();

			FCollisionQueryParams Params(NAME_None, false, Character);
			Params.bIgnoreTouches = true;
			Params.AddIgnoredActor(Character);
			//Look->GetCamera()->GetComponentRotation().
			Result = GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + AimRotation.Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params);
			if (Result)
				AttackHitResult = HitResult.ImpactPoint;
			//DrawDebugLine(GetWorld(), Character->GetCamera()->GetComponentLocation(), Character->GetCamera()->GetComponentLocation() + AimRotation.Vector() * 50000.f, FColor::Green, false, 2.f);
			UpdateLookAngles(Result ? UKismetMathLibrary::FindLookAtRotation(Tank->GetCenterLocation(), AttackHitResult) : Character->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentRotation(), DeltaSeconds);

			SetAttackInfo(Result, AttackHitResult, LookYawAngle, LookPitchAngle);

		}
	}

	if (IsAttack) GunAttack(IsAttack, !Result, AttackHitResult);
}

void UTankAnimComponent::UpdateLookAngles(const FRotator& Rotator, float DeltaSeconds)
{
	LookYawAngle = Tank->GetLookGun().Yaw;

	while (LookYawAngle - 180.f > Rotator.Yaw - 90.f)
	{
		LookYawAngle -= 360.f;
	}
	while (LookYawAngle + 180.f < Rotator.Yaw - 90.f)
	{
		LookYawAngle += 360.f;
	}
	LookYawAngle = FMath::FInterpTo(LookYawAngle, Rotator.Yaw - 90.f, DeltaSeconds, 20);

	LookPitchAngle = FMath::FInterpTo(LookPitchAngle, -Rotator.Pitch, DeltaSeconds, 20);
	if (LookPitchAngle > 20.f)
	{
		LookPitchAngle = 20.f;
	}
	else if (LookPitchAngle < -20.f)
	{
		LookPitchAngle = -20.f;
	}

	Tank->SetLookGun(LookYawAngle, LookPitchAngle);
}

void UTankAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Anim)
		TankAnim = Cast<UTankAnimInstance>(Character->GetMesh()->GetAnimInstance());

}

void UTankAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTankAnimComponent, Mode);
}

void UTankAnimComponent::OnRep_UpdateAnimationValues()
{
	if (!TankAnim) return;

	TankAnim->SetIsAttack(IsAttack);

	Super::OnRep_UpdateAnimationValues();

	TankAnim->Result = Result;

	TankAnim->Mode = Mode;

	TankAnim->AttackHitResult = AttackHitResult;
}

void UTankAnimComponent::Bind(ABaseCharacter* NewCharacter)
{
	Super::Bind(NewCharacter);

	Tank = Cast<ATank>(Character);
}

void UTankLv3AnimComponent::UpdateTankAnim(float DeltaSeconds)
{
	if (!IsValid(TankLv3))
		return;
	
	if (IsGLauncherMode)
	{
		ComponentLocation = TankLv3->GetCamera()->GetCameraComponent()->GetComponentLocation();

		FCollisionQueryParams Params(NAME_None, false, Character);
		Params.bIgnoreTouches = true;
		Params.AddIgnoredActor(Character);

		Result = GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + AimRotation.Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params);
		if (Result)
			AttackHitResult = HitResult.ImpactPoint;

		FRotator CharacterRotation = Character->GetActorRotation();

		//IsGLauncherMode ? TankLv3->GetCamera()->GetActorLocation() :

		auto Camera = Character->GetComponent<ULookComponent>(Component::Look)->GetCamera();
		Camera->SetWorldRotation(FRotator(FMath::Clamp(Camera->GetComponentRotation().Pitch, -20, 20), Camera->GetComponentRotation().Yaw, Camera->GetComponentRotation().Roll));

		UpdateLookAngles(FRotator(Character->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentRotation().Pitch, CharacterRotation.Yaw, 0.), DeltaSeconds);

		SetAttackInfo(Result, AttackHitResult, LookYawAngle, LookPitchAngle);
	}
	else Super::UpdateTankAnim(DeltaSeconds);

}

void UTankLv3AnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTankLv3AnimComponent, IsGLauncherMode);
}

void UTankLv3AnimComponent::OnRep_UpdateAnimationValues()
{
	Super::OnRep_UpdateAnimationValues();

	if(TankAnim) TankAnim->IsGLauncherMode = IsGLauncherMode;

}

void UTankLv3AnimComponent::Bind(ABaseCharacter* NewCharacter)
{
	Super::Bind(NewCharacter);

	TankLv3 = Cast<ATankLv3>(NewCharacter);
}

void UTrainerAnimComponent::SetRootYaw(float DeltaSeconds)
{
	Super::SetRootYaw(DeltaSeconds);

	FCollisionQueryParams Params(NAME_None, false, Trainer);
	Params.bIgnoreTouches = true;
	Params.AddIgnoredActor(Trainer);

	FRotator Target;
	if (RideMoose)
	{
		Params.AddIgnoredActor(RideMoose);

		FRotator MooseRotator = RideMoose->GetMesh()->GetSocketRotation("spine_2Socket");

		UMooseAnimInstance* MooseAnim = Cast<UMooseAnimInstance>(RideMoose->GetMesh()->GetAnimInstance());
		MooseBlend = MooseAnim->GetBlendAlpha();
		MooseSpeed = MooseAnim->GetGroundSpeed();
		RootYaw = MooseRotator.Yaw - Trainer->GetActorRotation().Yaw;

	}

	FHitResult HitResult;

	auto ComponentLocation = Trainer->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentLocation();

	if (GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + AimRotation.Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params))
	{
		AttackHitResult = HitResult.ImpactPoint;
		//DrawDebugLine(GetWorld(), Character->GetCamera()->GetComponentLocation(), HitResult.ImpactPoint, FColor::Green, false, 2.f);
		Target = UKismetMathLibrary::FindLookAtRotation(Trainer->GetActorLocation(), AttackHitResult);

		//Target.Pitch += -Trainer->GetActorRotation().Pitch;
		Target.Yaw -= Trainer->GetMesh()->GetComponentRotation().Yaw + 90.f;

		LookSky = false;
	}
	else
	{
		Target.Pitch = AimRotation.Pitch;//-Trainer->GetActorRotation().Pitch;
		if (Target.Pitch >= 270.f) Target.Pitch -= 360.f;
		Target.Yaw = AimRotation.Yaw - Trainer->GetMesh()->GetComponentRotation().Yaw - 90.f;
		LookSky = true;
	}

	//
	while (Target.Yaw < -180)
	{
		Target.Yaw += 360;
	}
	while (Target.Yaw > 180)
	{

		Target.Yaw -= 360;
	}
	BlendRotator.Pitch = FMath::FInterpTo(BlendRotator.Pitch, Target.Pitch, DeltaSeconds, 10.);
	BlendRotator.Yaw = FMath::FInterpTo(BlendRotator.Yaw, FMath::Clamp(Target.Yaw, -90, 90), DeltaSeconds, 10.);
	//UE_LOG(LogTemp, Log, TEXT("%f, %f"), BlendRotator.Pitch, BlendRotator.Yaw);
}

void UTrainerAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UTrainerAnimComponent, BlendRotator);

	//DOREPLIFETIME(UTrainerAnimComponent, AttackHitResult);
	//
	//DOREPLIFETIME(UTrainerAnimComponent, LookSky);

	DOREPLIFETIME(UTrainerAnimComponent, RideMoose);

	DOREPLIFETIME(UTrainerAnimComponent, IsRide);

	DOREPLIFETIME(UTrainerAnimComponent, MooseBlend);

	DOREPLIFETIME(UTrainerAnimComponent, MooseSpeed);
}

void UTrainerAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Anim)
		TrainerAnim = Cast<UTrainerAnimInstance>(Anim);
}

void UTrainerAnimComponent::OnRep_UpdateAnimationValues()
{
	Super::OnRep_UpdateAnimationValues();

	if (!TrainerAnim) return;

	TrainerAnim->BlendRotator = BlendRotator;

	TrainerAnim->AttackHitResult = AttackHitResult;

	TrainerAnim->LookSky = LookSky;

	TrainerAnim->RideMoose = RideMoose;

	TrainerAnim->IsRide = IsRide;

	TrainerAnim->MooseBlend = MooseBlend;

	TrainerAnim->MooseSpeed = MooseSpeed;

}

void UTrainerAnimComponent::Bind(ABaseCharacter* NewCharacter)
{
	Super::Bind(NewCharacter);

	Trainer = Cast<ATrainer>(NewCharacter);
}

void UEnemyAnimComponent::AILook(float DeltaSeconds)
{
	if (IsAI && (Target || IsLook) && !IsFailMovoTo)
	{
		FRotator TargetRotator;

		if (Target)
		{
			TargetRotator = UKismetMathLibrary::FindLookAtRotation(Character->GetMesh()->GetSocketLocation("headSocket"), Target->GetActorLocation());
			//RootYaw = FMath::FInterpTo(RootYaw, 0., DeltaSeconds, 7);
		}
		else
			TargetRotator = UKismetMathLibrary::FindLookAtRotation(Character->GetMesh()->GetSocketLocation("headSocket"), LookVector);

		auto CharacterRotaor = Character->GetActorRotation().Yaw;

		CharacterRotaor -= 180.;

		CharacterRotaor -= AimRotation.Yaw;

		while (CharacterRotaor < -180.)
		{
			CharacterRotaor += 360.;
		}

		while (CharacterRotaor > 180.)
		{
			CharacterRotaor -= 360.;
		}

		TargetRotator.Yaw -= AimRotation.Yaw;

		while (TargetRotator.Yaw < -180)
		{
			TargetRotator.Yaw += 360.;
		}
		while (TargetRotator.Yaw > 180)
		{
			TargetRotator.Yaw -= 360.;
		}

		if (CharacterRotaor < 0 && TargetRotator.Yaw < CharacterRotaor)
		{
			TargetRotator.Yaw += 360.;
		}
		else if (CharacterRotaor > 0 && TargetRotator.Yaw > CharacterRotaor)
		{
			TargetRotator.Yaw -= 360.;
		}

		LookRotator = FRotator(
			FMath::Clamp(FMath::FInterpTo(LookRotator.Pitch, TargetRotator.Pitch, DeltaSeconds, 3), -45., 45.),
			FMath::Clamp(FMath::FInterpTo(LookRotator.Yaw, TargetRotator.Yaw, DeltaSeconds, 3), -90., 90.),
			FMath::Clamp(FMath::FInterpTo(LookRotator.Roll, TargetRotator.Roll, DeltaSeconds, 3), -45., 45.));

	}
	else
	{
		LookRotator = FRotator(
			FMath::FInterpTo(LookRotator.Pitch, 0.f, DeltaSeconds, 20),
			FMath::FInterpTo(LookRotator.Yaw, 0.f, DeltaSeconds, 20),
			FMath::FInterpTo(LookRotator.Roll, 0.f, DeltaSeconds, 20));
	}
}

void UEnemyAnimComponent::AIMove()
{
	if (!IsAI) return;

	if (auto Move = Character->GetComponent<UMoveComponent>(Component::Move))
	{
		Move->SetForwardValue(IsMoving ? 1.f : 0.f);

		ForwardValue = Move->GetForwardValue();
	}
}

void UEnemyAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnemyAnimComponent, IsFailMovoTo);

	DOREPLIFETIME(UEnemyAnimComponent, IsLook);

	DOREPLIFETIME(UEnemyAnimComponent, Target);

	DOREPLIFETIME(UEnemyAnimComponent, LookVector);

	DOREPLIFETIME(UEnemyAnimComponent, LookRotator);
}

void UEnemyAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Anim)
		BaseEnemyAnim = Cast<UBaseEnemyAnimInstance>(Anim);
}

void UEnemyAnimComponent::UpdateAnimation(float DeltaSeconds)
{
	Super::UpdateAnimation(DeltaSeconds);

	AILook(DeltaSeconds);

	AIMove();
}

void UEnemyAnimComponent::OnRep_UpdateAnimationValues()
{
	Super::OnRep_UpdateAnimationValues();

	if (!BaseEnemyAnim) return;

	BaseEnemyAnim->IsLook = IsLook;

	BaseEnemyAnim->IsAI = IsAI;

	BaseEnemyAnim->Target = Target;

	BaseEnemyAnim->LookVector = LookVector;

	BaseEnemyAnim->LookRotator = LookRotator;
}

void UMooseAnimComponent::SetRootYaw(float DeltaSeconds)
{
	if (auto Move = Character->GetComponent<UMoveComponent>(Component::Move))
	{
		if (!IsAI)
		{
			FinalRotation = MovingRotation;
			MovingRotation = Character->GetActorRotation();
			RootYaw = RootYaw - UKismetMathLibrary::NormalizedDeltaRotator(MovingRotation, FinalRotation).Yaw;

			ForwardValue = Move->GetForwardValue();
			RightValue = Move->GetRightValue();
		}
		else
		{
			if (IsMoving)
			{
				while (RootYaw > 180.f)
				{
					RootYaw -= 360.f;
				}
				while (RootYaw < -180.f)
				{
					RootYaw += 360.f;
				}
				RootYaw = FMath::FInterpTo(RootYaw, 0, DeltaSeconds, 10);
			}
			else
			{
				FinalRotation = MovingRotation;
				MovingRotation = Character->GetActorRotation();
				RootYaw = RootYaw - UKismetMathLibrary::NormalizedDeltaRotator(MovingRotation, FinalRotation).Yaw;
			}
		}
		Character->GetMesh()->SetRelativeRotation(DefaultRotator + FRotator(0., RootYaw, 0.));
	}
}

void UMooseAnimComponent::SetIsAI()
{
	IsAI = Character->GetController() && !Character->GetController()->IsPlayerController() && !Character->Owner->IsA(ATrainer::StaticClass());
}

void UMooseAnimComponent::AddRootYawServer_Implementation(float value)
{
	RootYaw += value;
}

void UMooseAnimComponent::AddRootYaw(float value)
{
	RootYaw += value; 

	if(!Character->HasAuthority())
		AddRootYawServer(value);
}

void URockMonsterAnimComponent::UpdateAnimation(float DeltaSeconds)
{
	Super::UpdateAnimation(DeltaSeconds);

	if (!IsAI) return;

	if (auto Move = Character->GetComponent<UMoveComponent>(Component::Move))
	{
		!IsFailMovoTo && Target ? Move->SetCrouch<false>() : Move->SetCrouch<true>();

		if (IsCrouch) IsAttack = false;
	}
}