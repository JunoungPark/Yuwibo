// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAnimInstance.h"
#include "Yuwibo/Character/TankLv3.h"
#include "Camera/CameraActor.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/Component/FootIkComponent.h"


void UTankAnimInstance::AnimNotify_ToRoll()
{
	if (Tank->HasAuthority()) Tank->GetComponent<UMoveComponent>(Component::Move)->SetCanMove(true);

	Tank->SetRollSound();
}

void UTankAnimInstance::AnimNotify_ToSpider()
{
	if (!Tank->HasAuthority()) return;
		
	Tank->GetComponent<UMoveComponent>(Component::Move)->SetCanMove(true);
}

void UTankAnimInstance::AnimNotify_LandStart()
{
	if (!Tank->HasAuthority()) return;

	Tank->GetComponent<UMoveComponent>(Component::Move)->SetCanMove(false);
}

void UTankAnimInstance::AnimNotify_LandEnd()
{
	if (!Tank->HasAuthority()) return;

	if (!(TankLv3 && TankLv3->GetComponent<UTankLv3AnimComponent>(Component::Anim)->GetGLauncherMode())) Tank->GetComponent<UMoveComponent>(Component::Move)->SetCanMove(true);
}

void UTankAnimInstance::AnimNotify_Roll()
{
	Tank->Noise();
}

void UTankAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Tank = Cast<ATank>(Character);
	TankLv3 = Cast<ATankLv3>(Tank);
}
//void UTankAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	Super::NativeUpdateAnimation(DeltaSeconds);
//		
//	if (!IsValid(Tank))
//		return;
//	if (!Character->IsLocallyControlled() && !Character->HasAuthority()) return;
//
//	IsGLauncherMode = TankLv3 && TankLv3->GetGLauncherMode();
//	if (IsAttack || Character->GetCharacterMovement()->Velocity.SizeSquared() > 0.f || (IsGLauncherMode))
//	{
//		if (IsGLauncherMode)
//		{
//			ComponentLocation = TankLv3->GetCamera()->GetCameraComponent()->GetComponentLocation();
//
//			FCollisionQueryParams Params(NAME_None, false, Character);
//			Params.bIgnoreTouches = true;
//			Params.AddIgnoredActor(Character);
//
//			Result = GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + AimRotation.Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params);
//			if (Result)
//				AttackHitResult = HitResult.ImpactPoint;
//
//			FRotator CharacterRotation = Character->GetActorRotation();
//
//			//IsGLauncherMode ? TankLv3->GetCamera()->GetActorLocation() :
//			
//			
//			auto Camera = Character->GetComponent<ULookComponent>(Component::Look)->GetCamera();
//			Camera->SetWorldRotation(FRotator(FMath::Clamp(Camera->GetComponentRotation().Pitch, -20, 20), Camera->GetComponentRotation().Yaw, Camera->GetComponentRotation().Roll));
//
//			UpdateLookAngles(FRotator(Character->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentRotation().Pitch, CharacterRotation.Yaw, 0.), DeltaSeconds);
//			
//			Tank->SetLookGun(LookYawAngle, LookPitchAngle);
//
//		}
//		else
//		{
//			ComponentLocation = Character->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentLocation();
//
//			FCollisionQueryParams Params(NAME_None, false, Character);
//			Params.bIgnoreTouches = true;
//			Params.AddIgnoredActor(Character);
//			//Look->GetCamera()->GetComponentRotation().
//			Result = GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + AimRotation.Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params);
//			if (Result)
//				AttackHitResult = HitResult.ImpactPoint;
//			//DrawDebugLine(GetWorld(), Character->GetCamera()->GetComponentLocation(), Character->GetCamera()->GetComponentLocation() + AimRotation.Vector() * 50000.f, FColor::Green, false, 2.f);
//			UpdateLookAngles(Result ? UKismetMathLibrary::FindLookAtRotation(Tank->GetCenterLocation(), AttackHitResult) : Character->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentRotation(), DeltaSeconds);
//
//			Tank->SetLookGun(LookYawAngle, LookPitchAngle);
//
//			if (IsAttack) OnOffAttack.Broadcast(IsAttack, !Result, AttackHitResult);
//		}
//
//	}
//
//}
//void UTankAnimInstance::UpdateLookAngles(const FRotator& Rotator, float DeltaSeconds)
//{
//	LookYawAngle = Tank->GetLookGun().Yaw;
//
//	while (LookYawAngle - 180.f > Rotator.Yaw - 90.f)
//	{
//		LookYawAngle -= 360.f;
//	}
//	while (LookYawAngle + 180.f < Rotator.Yaw - 90.f)
//	{
//		LookYawAngle += 360.f;
//	}
//	LookYawAngle = FMath::FInterpTo(LookYawAngle, Rotator.Yaw - 90.f, DeltaSeconds, 20);
//
//	LookPitchAngle = FMath::FInterpTo(LookPitchAngle, -Rotator.Pitch, DeltaSeconds, 20);
//	if (LookPitchAngle > 20.f)
//	{
//		LookPitchAngle = 20.f;
//	}
//	else if (LookPitchAngle < -20.f)
//	{
//		LookPitchAngle = -20.f;
//	}
//}
//void UTankAnimInstance::CharacterTurn(float DeltaSeconds)
//{
//	
//	Mode = Tank->GetMode();
//	if (Tank->GetComponent<UMoveComponent>(Component::Move)->GetCanMove())
//	{
//		if (Mode == true)
//		{
//			if (IsMoving)
//			{
//				if (RootYaw - 180.f > UKismetMathLibrary::MakeRotFromX(Velocity).Yaw - 90.f)
//				{
//					RootYaw -= 360.f;
//				}
//				else if (RootYaw + 180.f < UKismetMathLibrary::MakeRotFromX(Velocity).Yaw - 90.f)
//				{
//					RootYaw += 360.f;
//				}
//				RootYaw = FMath::FInterpTo(RootYaw, UKismetMathLibrary::MakeRotFromX(Velocity).Yaw - 90.f, DeltaSeconds, 10);
//
//				if (!Tank->GetRollSound())
//				{
//					Tank->SetRollSound();
//				}
//			}
//			else
//			{
//				if (Tank->GetRollSound())
//				{
//					Tank->SetRollSound();
//				}
//			}
//		}
//		else
//		{
//
//			if (IsMoving)
//			{
//				YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(Velocity), FRotator(0.f, RootYaw, 0.f)).Yaw - 90.f;
//				if (YawOffset < -180.f)
//				{
//					YawOffset += 360.f;
//				}
//				else if (YawOffset > 180.f)
//				{
//					YawOffset -= 360.f;
//				}
//			}
//		}
//	}
//	
//
//	Tank->GetMesh()->SetWorldRotation(FRotator(0.f, RootYaw, 0.f));
//	
//	
//}

void UTankAnimInstance::FootIk(FFourFootIkStruct FootStruct)
{
	HipOffset = FVector(0., 0., FootStruct.HipOffset);

	LeftFootRotatorValue = FootStruct.LeftFootRotatorValue;

	RightFootRotatorValue = FootStruct.RightFootRotatorValue;

	BackLeftFootRotatorValue = FootStruct.BackLeftFootRotatorValue;

	BackRightFootRotatorValue = FootStruct.BackRightFootRotatorValue;

	LeftFootEffectorLocation = FVector(FootStruct.LeftFootOffset * -3.5, 0., 0.);

	RightFootEffectorLocation = FVector(FootStruct.RightFootOffset * 3.5, 0., 0.);

	if (Mode == true)
	{

		BackLeftFootEffectorLocation = FVector(0., FootStruct.BackLeftFootOffset * -2.5, 0.);

		BackRightFootEffectorLocation = FVector(0., FootStruct.BackRightFootOffset * 2.5, 0.);
	}
	else
	{
		
		BackLeftFootEffectorLocation = FVector(FootStruct.BackLeftFootOffset * -3.5, 0., 0.);

		BackRightFootEffectorLocation = FVector(FootStruct.BackRightFootOffset * 3.5, 0., 0.);
	}
}

void UTankAnimInstance::SetIsAttack(bool IsTrue)
{
	if (IsAttack == IsTrue)
		return;

	IsAttack = IsTrue;
	if(!IsAttack) OnOffAttack.Broadcast(false, false, FVector());
}

void UTankAnimInstance::GunAttack(bool IsTrue, bool IsLookSky, FVector Vector)
{
	OnOffAttack.Broadcast(IsTrue, IsLookSky, Vector);
}

void UTankAnimInstance::GLauncherAttack()
{
	if (IsGLauncherMode) OnGLauncherAttack.Broadcast(!Result, AttackHitResult);
}
