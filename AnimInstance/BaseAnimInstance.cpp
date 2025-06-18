// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "Yuwibo/Character/MoveCharacter.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"

void UBaseAnimInstance::AnimNotify_Search()
{
	Character->GetComponent<UAnimComponent>(Component::Anim)->SetAttackYaw();
}

void UBaseAnimInstance::AnimNotify_EndMontageMove()
{
	Character->GetComponent<UAnimComponent>(Component::Anim)->SetIsAttack(false);
}

void UBaseAnimInstance::AnimNotify_EndAttack()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Character->GetComponent<UMoveComponent>(Component::Move)->SetAttack<false>();
		BlendAlpha = 0.f;
	}

}
void UBaseAnimInstance::AnimNotify_FootStep()
{
	if (AMoveCharacter* Move = Cast<AMoveCharacter>(Character))
	{
		Move->PlayFootSound();
		Move->Noise();
	}
}
void UBaseAnimInstance::NativeInitializeAnimation()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Character = Cast<ABaseCharacter>(Pawn);
		if (IsValid(Character))
		{
			//CharacterMovement = Character->GetCharacterMovement();

			if (auto MyGameInstance = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				AttackMontage = MyGameInstance->GetCharacterData(Character->GetCharacterName())->AttackMontage.LoadSynchronous();
				//RotationToMove = MyGameInstance->GetCharacterData(Character->GetCharacterName())->RotationToMovement;
			}
		}
	}

}

//void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	//if (IsValid(CharacterMovement))
//	//{
//	//
//	//
//	//	//IsFalling = CharacterMovement->IsFalling();
//	//	//
//	//	//
//	//	//Velocity = CharacterMovement->Velocity;
//	//	////UE_LOG(LogTemp, Log, TEXT("%f, %f, %f"), Velocity.X, Velocity.Y, Velocity.Z);
//	//	//
//	//	//GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
//	//	//
//	//	//if (GroundSpeed > 0.f)
//	//	//	IsMoving = true;
//	//	//else
//	//	//	IsMoving = false;
//	//	//
//	//	//AimRotation = Character->GetBaseAimRotation();
//	//	//
//	//	//BlendAlpha = FMath::FInterpTo(BlendAlpha, GroundSpeed / CharacterMovement->GetMaxSpeed(), DeltaSeconds, 20);
//	//	//YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(Velocity), AimRotation).Yaw;
//	//
//	//	//ForwardValue = Character->GetForwardValue();
//	//	//RightValue = Character->GetRightValue();
//	//	//UE_LOG(LogTemp, Log, TEXT("%f"), RightValue);
//	//
//	//	//CharacterTurn(DeltaSeconds);
//	//	//FootIk();
//	//	
//	//	
//	//}
//}

//void UBaseAnimInstance::CharacterTurn(float DeltaSeconds)
//{
//	if (Montage_IsPlaying(AttackMontage))
//	{
//		RootYaw = FMath::FInterpTo(RootYaw, AttackYaw, DeltaSeconds, 20);
//
//		FinalRotation = MovingRotation;
//		MovingRotation = Character->GetActorRotation();
//
//		AttackYaw = AttackYaw - UKismetMathLibrary::NormalizedDeltaRotator(MovingRotation, FinalRotation).Yaw;
//
//	}
//	else if (IsMoving)
//	{
//		if (!RotationToMove || (Character->GetController() && !Character->GetController()->IsPlayerController()))
//		{
//			while (RootYaw > 180.f)
//			{
//				RootYaw -= 360.f;
//			}
//			while (RootYaw < -180.f)
//			{
//				RootYaw += 360.f;
//			}
//			RootYaw = FMath::FInterpTo(RootYaw, 0, DeltaSeconds, 10);
//		}
//		else 
//		{
//			while (RootYaw > YawOffset + 180.f)
//			{
//				YawOffset += 360.f;
//			}
//			while (RootYaw < YawOffset - 180.f)
//			{
//				YawOffset -= 360.f;
//			}
//			RootYaw = FMath::FInterpTo(RootYaw, YawOffset, DeltaSeconds, 10);
//
//
//		}
//		
//
//		//if (Character->GetCharacterName() == "Vampire")
//		//{
//		//	UE_LOG(LogTemp, Log, TEXT("%f %f"), RootYaw, YawOffset);
//		//}
//		MovingRotation = Character->GetActorRotation();
//		FinalRotation = MovingRotation;
//	}
//	else
//	{
//		FinalRotation = MovingRotation;
//		MovingRotation = Character->GetActorRotation();
//		RootYaw = RootYaw - UKismetMathLibrary::NormalizedDeltaRotator(MovingRotation, FinalRotation).Yaw;
//	}
//}

void UBaseAnimInstance::PlayAttackAnimation()
{
	if (IsValid(AttackMontage))
	{
		if (!Montage_IsPlaying(AttackMontage))
		{
			Montage_Play(AttackMontage, 1.f);
		}
		else if (Montage_GetCurrentSection(AttackMontage) == FName("Attack_0"))
		{
			Montage_SetNextSection(FName("Attack_0"), FName("Attack_1"));
		}
		else if (Montage_GetCurrentSection(AttackMontage) == FName("Attack_1"))
		{
			Montage_SetNextSection(FName("Attack_1"), FName("Attack_2"));
		}
	}
}

void UBaseAnimInstance::PlayAttackAnimation(float Position)
{
	if (IsValid(AttackMontage))
	{

		if (!Montage_IsPlaying(AttackMontage))
		{
			Montage_Play(AttackMontage, 1.f);
		}
		
		Montage_SetPosition(AttackMontage, Position);
	
	}
}

void UBaseAnimInstance::DebugCapsule(float AttackRange, float CapsuleHalfHeight, FVector StartOffset)
{
	DrawDebugCapsule(GetWorld(), Character->GetActorLocation() + Character->GetActorForwardVector() * AttackRange * 0.5 + StartOffset, AttackRange * 0.5 + Character->GetCapsuleComponent()->GetScaledCapsuleRadius(), CapsuleHalfHeight, FRotationMatrix::MakeFromZ(Character->GetActorForwardVector() * AttackRange).ToQuat(), FColor::Green, false, 10.f);
}

void UTwoFootAnimInstance::FootIk(FTwoFootIkStruct FootStruct)
{

	HipOffset = FVector(0., 0., FootStruct.HipOffset);

	LeftFootRotatorValue = FootStruct.LeftFootRotatorValue;

	RightFootRotatorValue = FootStruct.RightFootRotatorValue;

	LeftFootEffectorLocation = FVector(FootStruct.LeftFootOffset, 0., 0.);

	RightFootEffectorLocation = FVector(-FootStruct.RightFootOffset, 0., 0.);

}

void UFourFootAnimInstance::FootIk(FFourFootIkStruct FootStruct)
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
