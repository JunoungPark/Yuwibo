// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAnimInstance.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/Component/FootIkComponent.h"


void UBaseEnemyAnimInstance::AnimNotify_Search()
{
	if(IsAI)
	{
		if (Target)
		{
			Character->SetActorRotation(FRotator(Character->GetActorRotation().Pitch, UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), Target->GetActorLocation()).Yaw, Character->GetActorRotation().Roll));
		}
		else
		{
			Montage_Play(AttackMontage, 1.f);
		}
	}
	
	Super::AnimNotify_Search();
	
}

//void UBaseEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	
//	Super::NativeUpdateAnimation(DeltaSeconds);
//
//	if (!IsValid(CharacterMovement)) return;
//	
//	IsPlayer = Character->IsPlayerControlled();
//	
//	AILook(DeltaSeconds);
//	
//	AIMove();
//}

//void UBaseEnemyAnimInstance::SetLook(bool IsTrue, FVector Vector)
//{
//	IsLook = IsTrue;
//
//	LookVector = Vector;
//}

//void UBaseEnemyAnimInstance::AILook(float DeltaSeconds)
//{
//
	//if (!IsPlayer && (Target || IsLook))
	//{
	//	FRotator TargetRotator;
	//
	//	if (Target)
	//	{
	//		TargetRotator = UKismetMathLibrary::FindLookAtRotation(Character->GetMesh()->GetSocketLocation("headSocket"), Target->GetActorLocation());
	//		//RootYaw = FMath::FInterpTo(RootYaw, 0., DeltaSeconds, 7);
	//	}
	//	else
	//		TargetRotator = UKismetMathLibrary::FindLookAtRotation(Character->GetMesh()->GetSocketLocation("headSocket"), LookVector);
	//
	//	auto CharacterRotaor = Character->GetActorRotation().Yaw;
	//
	//	CharacterRotaor -= 180.;
	//
	//	CharacterRotaor -= AimRotation.Yaw;
	//
	//	while (CharacterRotaor < -180.)
	//	{
	//		CharacterRotaor += 360.;
	//	}
	//
	//	while (CharacterRotaor > 180.)
	//	{
	//		CharacterRotaor -= 360.;
	//	}
	//
	//	TargetRotator.Yaw -= AimRotation.Yaw;
	//
	//	while (TargetRotator.Yaw < -180)
	//	{
	//		TargetRotator.Yaw += 360.;
	//	}
	//	while (TargetRotator.Yaw > 180)
	//	{
	//		TargetRotator.Yaw -= 360.;
	//	}
	//
	//	if (CharacterRotaor < 0 && TargetRotator.Yaw < CharacterRotaor)
	//	{
	//		TargetRotator.Yaw += 360.;
	//	}
	//	else if (CharacterRotaor > 0 && TargetRotator.Yaw > CharacterRotaor)
	//	{
	//		TargetRotator.Yaw -= 360.;
	//	}
	//
	//	LookRotator = FRotator(
	//		FMath::Clamp(FMath::FInterpTo(LookRotator.Pitch, TargetRotator.Pitch, DeltaSeconds, 3), -45., 45.),
	//		FMath::Clamp(FMath::FInterpTo(LookRotator.Yaw, TargetRotator.Yaw, DeltaSeconds, 3), -90., 90.),
	//		FMath::Clamp(FMath::FInterpTo(LookRotator.Roll, TargetRotator.Roll, DeltaSeconds, 3), -45., 45.));
	//
	//}
	//else
	//{
	//	LookRotator = FRotator(
	//		FMath::FInterpTo(LookRotator.Pitch, 0.f, DeltaSeconds, 20),
	//		FMath::FInterpTo(LookRotator.Yaw, 0.f, DeltaSeconds, 20),
	//		FMath::FInterpTo(LookRotator.Roll, 0.f, DeltaSeconds, 20));
	//}
//
	//if (!(Character->Owner == UGameplayStatics::GetPlayerController(Character, 0) || Cast<ATrainer>(Character->Owner)))
	//{
	//	RootYaw = 0;
	//	if (IsMoving)
	//	{
	//		ForwardValue = 1.f;
	//	}
	//}
	//UEnemyComponent* Enemy = Character->GetComponent<UEnemyComponent>(Component::Enemy);
	//LookCharacter = Enemy->GetLookCharacter();
	//BaseLookRotator = Enemy->GetLookRotator();
	//BaseLookRotator.Pitch = -BaseLookRotator.Pitch;
	//BaseLookRotator.Yaw -= AimRotation.Yaw;
	//BodyRotator = Enemy->GetBodyRotator();
	//if (LookCharacter)
	//{
	//	if (!IsFirstTurn && !Montage_IsPlaying(AttackMontage))
	//	{
	//		if (Character->GetActorRotation().Yaw > BodyRotator.Yaw + 180.f)
	//		{
	//			BodyRotator.Yaw += 360.f;
	//		}
	//		else if (Character->GetActorRotation().Yaw < BodyRotator.Yaw - 180.f)
	//		{
	//			BodyRotator.Yaw -= 360.f;
	//
	//		}
	//		Character->SetActorRotation(FRotator(Character->GetActorRotation().Pitch,FMath::FInterpTo(Character->GetActorRotation().Yaw, BodyRotator.Yaw, DeltaSeconds, 3), Character->GetActorRotation().Roll));
	//		
	//
	//		float SpineYaw = BodyRotator.Yaw - Character->GetActorRotation().Yaw;
	//		if (SpineYaw < SpineRotator.Yaw - 180)
	//		{
	//			SpineYaw += 360.f;
	//		}
	//		else if (SpineYaw > SpineRotator.Yaw + 180)
	//		{
	//			SpineYaw -= 360.f;
	//		}
	//		SpineRotator = FRotator(0., FMath::FInterpTo(SpineRotator.Yaw, SpineYaw, DeltaSeconds, 5), 0.);
	//	}
	//	float LookYaw = BaseLookRotator.Yaw - SpineRotator.Yaw;
	//	if (LookYaw < LookRotator.Yaw - 180)
	//	{
	//		LookYaw += 360.f;
	//	}
	//	else if (LookYaw > LookRotator.Yaw + 180)
	//	{
	//		LookYaw -= 360.f;
	//	}
	//	LookRotator = FRotator(FMath::Clamp(FMath::FInterpTo(LookRotator.Pitch, BaseLookRotator.Pitch, DeltaSeconds, 10), -45., 45.), FMath::Clamp(FMath::FInterpTo(LookRotator.Yaw, LookYaw, DeltaSeconds, 10), -45., 45.), FMath::FInterpTo(LookRotator.Roll, BaseLookRotator.Roll, DeltaSeconds, 10));
	//
	//}
	//else
	//{
	//	LookRotator = FRotator(FMath::FInterpTo(LookRotator.Pitch, 0, DeltaSeconds, 20), FMath::FInterpTo(LookRotator.Yaw, 0, DeltaSeconds, 20), FMath::FInterpTo(LookRotator.Roll, 0, DeltaSeconds, 20));
	//	SpineRotator = FRotator(0., FMath::FInterpTo(SpineRotator.Yaw, 0, DeltaSeconds, 5), 0.);
	//}
	//
	//if (LookCharacter && !Character->GetComponent<UMoveComponent>(Component::Move)->GetCanMove())
	//{
	//	BlendAlpha = FMath::FInterpTo(BlendAlpha, 1, DeltaSeconds, 20);
	//	YawOffset = 0;
	//}
//}

//void UBaseEnemyAnimInstance::AIMove()
//{
//	if (IsPlayer) return;
//
//	if (auto Move = Character->GetComponent<UMoveComponent>(Component::Move))
//	{
//		Move->SetForwardValue(IsMoving ? 1.f : 0.f);
//
//		ForwardValue = Move->GetForwardValue();
//	}
//}

void UTwoFootEnemyAnimInstance::FootIk(FTwoFootIkStruct FootStruct)
{

	HipOffset = FVector(0., 0., FootStruct.HipOffset);

	LeftFootRotatorValue = FootStruct.LeftFootRotatorValue;

	RightFootRotatorValue = FootStruct.RightFootRotatorValue;

	LeftFootEffectorLocation = FVector(FootStruct.LeftFootOffset, 0., 0.);

	RightFootEffectorLocation = FVector(-FootStruct.RightFootOffset, 0., 0.);
}

void UFourFootEnemyAnimInstance::FootIk(FFourFootIkStruct FootStruct)
{

	HipOffset = FVector(0., 0., FootStruct.HipOffset);

	LeftFootRotatorValue = FootStruct.LeftFootRotatorValue;

	RightFootRotatorValue = FootStruct.RightFootRotatorValue;

	BackLeftFootRotatorValue = FootStruct.BackLeftFootRotatorValue;

	BackRightFootRotatorValue = FootStruct.BackRightFootRotatorValue;

	LeftFootEffectorLocation = FVector(FootStruct.LeftFootOffset * -3.5, 0., 0.);

	RightFootEffectorLocation = FVector(FootStruct.RightFootOffset * 3.5, 0., 0.);

	BackLeftFootEffectorLocation = FVector(FootStruct.BackLeftFootOffset * -3.5, 0., 0.);

	BackRightFootEffectorLocation = FVector(FootStruct.BackRightFootOffset * 3.5, 0., 0.);
}
