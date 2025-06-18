// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierAnimInstance.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Character/Soldier.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kismet/GameplayStatics.h"

void USoldierAnimInstance::AnimNotify_EndAttack()
{
	Character->GetComponent<UMoveComponent>(Component::Move)->SetAttack<false>();
}

//void USoldierAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	Super::NativeUpdateAnimation(DeltaSeconds);
//	if (Character)
//	{
//		FRotator CharacterRotation = Character->GetActorRotation();
//		HeadRotator.Pitch = AimRotation.Roll - CharacterRotation.Roll;
//
//		HeadRotator.Yaw = 0.;
//
//		HeadRotator.Roll = -FMath::Clamp(AimRotation.Pitch - CharacterRotation.Pitch + (AimRotation.Pitch - CharacterRotation.Pitch <= 90 ? 0 : -360), -50., 90.);
//
//		HandRotator.Pitch = 0.;
//
//		HandRotator.Yaw = FMath::Clamp(AimRotation.Yaw - CharacterRotation.Yaw - RootYaw + 10, -80., 100.);
//
//		HandRotator.Roll = -FMath::Clamp(AimRotation.Pitch - CharacterRotation.Pitch + 40, -45., 95.);
//
//		if (IsAttack)
//		{
//			BlendWeight = FMath::FInterpTo(BlendWeight, 1, DeltaSeconds, 10);
//		}
//		else
//		{
//			BlendWeight = FMath::FInterpTo(BlendWeight, 0, DeltaSeconds, 10);
//		}
//	}
//}

//void USoldierAnimInstance::CharacterTurn(float DeltaSeconds)
//{
//	CameraFinalYaw = CameraMovingYaw;
//	CameraMovingYaw = AimRotation.Yaw;
//	if (!IsMoving)
//	{
//		if (CameraFinalYaw < CameraMovingYaw - 1.5f)
//		{
//			IsRightTurn = true;
//			IsLeftTurn = false;
//		}
//		else if (CameraFinalYaw > CameraMovingYaw + 1.5f)
//		{
//			IsLeftTurn = true;
//			IsRightTurn = false;
//		}
//		else
//		{
//			IsLeftTurn = false;
//			IsRightTurn = false;
//		}
//	}
//	else
//	{
//		IsLeftTurn = false;
//		IsRightTurn = false;
//	}
//	
//}
