// Fill out your copyright notice in the Description page of Project Settings.


#include "MooseAnimInstance.h"
#include "Yuwibo/Character/Trainer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//void UMooseAnimInstance::CharacterTurn(float DeltaSeconds)
//{
//	if (auto Move = Character->GetComponent<UMoveComponent>(Component::Move))
//	{
//		if (IsPlayer)
//		{
//			FinalRotation = MovingRotation;
//			MovingRotation = Character->GetActorRotation();
//			RootYaw = RootYaw - UKismetMathLibrary::NormalizedDeltaRotator(MovingRotation, FinalRotation).Yaw;
//
//			ForwardValue = Move->GetForwardValue();
//			RightValue = Move->GetRightValue();
//		}
//		else
//		{
//			if (IsMoving)
//			{
//				while (RootYaw > 180.f)
//				{
//					RootYaw -= 360.f;
//				}
//				while (RootYaw < -180.f)
//				{
//					RootYaw += 360.f;
//				}
//				RootYaw = FMath::FInterpTo(RootYaw, 0, DeltaSeconds, 10);
//			}
//			else
//			{
//				FinalRotation = MovingRotation;
//				MovingRotation = Character->GetActorRotation();
//				RootYaw = RootYaw - UKismetMathLibrary::NormalizedDeltaRotator(MovingRotation, FinalRotation).Yaw;
//			}
//		}
//	}
//}
//void UMooseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	Super::Super::NativeUpdateAnimation(DeltaSeconds);
//
//	if (!IsValid(Character) || !IsValid(Character->Owner)) return;
//
//	IsPlayer = Character->IsPlayerControlled() || Character->Owner->IsA(ATrainer::StaticClass());
//
//	AILook(DeltaSeconds);
//
//	AIMove();
//}

void UMooseAnimInstance::MooseFootIk(FFourFootIkStruct FootStruct)
{
	HipOffset = FVector(0., 0., FootStruct.HipOffset);

	LeftFootRotatorValue = FootStruct.LeftFootRotatorValue + FRotator(0., -RootYaw, 0.);

	RightFootRotatorValue = FootStruct.RightFootRotatorValue + FRotator(0., -RootYaw, 0.);

	BackLeftFootRotatorValue = FootStruct.BackLeftFootRotatorValue + FRotator(0., -RootYaw, 0.);

	BackRightFootRotatorValue = FootStruct.BackRightFootRotatorValue + FRotator(0., -RootYaw, 0.);

	LeftFootEffectorLocation = FVector(-FootStruct.LeftFootOffset * 1.275, 0, 0.);

	RightFootEffectorLocation = FVector(FootStruct.RightFootOffset * 1.35, 0, 0.);

	BackLeftFootEffectorLocation = FVector(-FootStruct.BackLeftFootOffset*1.35, 0., FootStruct.BackLeftFootOffset * -0.405);

	BackRightFootEffectorLocation = FVector(FootStruct.BackRightFootOffset*1.575, 0., FootStruct.BackRightFootOffset * 0.4725);
}