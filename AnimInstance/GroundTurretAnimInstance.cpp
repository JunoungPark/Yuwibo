// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTurretAnimInstance.h"
#include "Yuwibo/Character/GroundTurret.h"
#include "Kismet/KismetMathLibrary.h"
#include "Yuwibo/Component/AiComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"

void UGroundTurretAnimInstance::AnimNotify_Attack()
{
	if (IsAttack && BlendAlpha == 1)
	{
		Turret->GetBullet(Turret->GetMesh()->GetBoneLocation("gun001"), GunRotation);
	}
}

void UGroundTurretAnimInstance::NativeInitializeAnimation()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Turret = Cast<AGroundTurret>(Pawn);
		//if (IsValid(Turret))
		//{
		//	CharacterMovement = Turret->GetCharacterMovement();
		//	
		//}
	}
}

//void UGroundTurretAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	//if (IsValid(CharacterMovement))
//	//{
//	//	CharacterTurn(DeltaSeconds);
//	//
//	//	if (IsAttack && BlendAlpha != 1)
//	//	{
//	//		BlendAlpha = FMath::FInterpTo(BlendAlpha, 1, DeltaSeconds, 10);
//	//	}
//	//	else if (!IsAttack && BlendAlpha != 0)
//	//	{
//	//		BlendAlpha = FMath::FInterpTo(BlendAlpha, 0, DeltaSeconds, 10);
//	//	}
//	//
//	//
//	//	GunRotation = Turret->GetGunRotation();
//	//
//	//	
//	//	
//	//}
//}
//
//void UGroundTurretAnimInstance::CharacterTurn(float DeltaSeconds)
//{
//	Turret->GetMesh()->SetWorldRotation(FRotator(0.f, RootYaw, 0.f));
//
//}

void UGroundTurretAnimInstance::FootIk(FFourFootIkStruct FootStruct)
{
	HipOffset = FVector(0., 0., FootStruct.HipOffset);

	LeftFootRotatorValue = FootStruct.LeftFootRotatorValue;

	RightFootRotatorValue = FootStruct.RightFootRotatorValue;

	BackLeftFootRotatorValue = FootStruct.BackLeftFootRotatorValue;

	BackRightFootRotatorValue = FootStruct.BackRightFootRotatorValue;

	LeftFootEffectorLocation = FVector(-FootStruct.LeftFootOffset * 0.5, 0., 0.);

	RightFootEffectorLocation = FVector(-FootStruct.RightFootOffset * 0.5, 0., 0.);

	BackLeftFootEffectorLocation = FVector(-FootStruct.BackLeftFootOffset * 0.5, 0., 0.);

	BackRightFootEffectorLocation = FVector(-FootStruct.BackRightFootOffset * 0.5, 0., 0.);
}
