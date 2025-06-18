// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGunAnimInstance.h"
#include "Yuwibo/Character/Tank.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Actor/Projectile/Bullet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/GunDefaultCameraShakeBase.h"
void UTankGunAnimInstance::AnimNotify_LeftGunAttack()
{
	if(IsAttack)
	{
		Character->GetBullet(Gun->GetSocketLocation(TEXT("Barrel_End_2")), LookSky ? Character->GetBaseAimRotation() : UKismetMathLibrary::FindLookAtRotation(Gun->GetSocketLocation(TEXT("Barrel_End_2")), TargetLocation));

		Character->ResetLeftFx();

		Character->PlayGunSound();
	}
}
void UTankGunAnimInstance::AnimNotify_RightGunAttack()
{
	if (IsAttack)
	{
		Character->GetBullet(Gun->GetSocketLocation(TEXT("Barrel_End_1")), LookSky ? Character->GetBaseAimRotation() : UKismetMathLibrary::FindLookAtRotation(Gun->GetSocketLocation(TEXT("Barrel_End_1")), TargetLocation));

		Character->ResetRightFx();

		Character->PlayGunSound();
	}
}
void UTankGunAnimInstance::NativeInitializeAnimation()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Character = Cast<ATank>(Pawn);
		Gun = GetOwningComponent();
	}

}

void UTankGunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!Character) return;

	if (!Character->IsLocallyControlled() && !Character->HasAuthority()) return;

	if (IsAttack)
	{
		//DrawDebugLine(GetWorld(), Character->GetCamera()->GetComponentLocation(), Character->GetCamera()->GetComponentLocation() + AimRotation.Vector() * 50000.f, FColor::Green, false, 2.f);
		
		UpdateLookAngles(LookSky ? Character->GetBaseAimRotation() : UKismetMathLibrary::FindLookAtRotation(Gun->GetComponentLocation(), TargetLocation), DeltaSeconds);
		if(Character->HasAuthority()) UE_LOG(LogTemp, Log, TEXT("%f"), Delta);
		if (UKismetMathLibrary::Abs(Delta) > 5)
		{
			IsAttack = false;
		}

	}
}

void UTankGunAnimInstance::UpdateLookAngles(FRotator Rotator, float DeltaSeconds)
{
	if (Loc == Location::Center)
	{
		if (Character->GetLookGun().Yaw - 180.f > Rotator.Yaw - 90.f)
		{
			Rotator.Yaw += 360.f;
		}
		else if (Character->GetLookGun().Yaw + 180.f < Rotator.Yaw - 90.f)
		{
			Rotator.Yaw -= 360.f;
		}
		Delta = Character->GetLookGun().Yaw - Rotator.Yaw + 90.f;
	
		return;
	}

	if (LookRollAngle > Rotator.Pitch + 180.f)
	{
		LookRollAngle -= 360.f;
	}
	else if (LookRollAngle < Rotator.Pitch - 180.f)
	{
		LookRollAngle += 360.f;
	}
	LookRollAngle = FMath::FInterpTo(LookRollAngle, Rotator.Pitch, DeltaSeconds, 20);

	LookYawAngle = Gun->GetComponentRotation().Yaw;

	if (LookYawAngle > 0.f && LookYawAngle - 180.f > Rotator.Yaw - 90.f)
	{
		LookYawAngle -= 360.f;
	}
	else if (LookYawAngle < 0.f && LookYawAngle + 180.f < Rotator.Yaw - 90.f)
	{
		LookYawAngle += 360.f;
	}

	LookYawAngle = FMath::FInterpTo(LookYawAngle, Rotator.Yaw - 90.f, DeltaSeconds, 20);

	auto ParentYaw = Gun->GetAttachParent()->GetComponentRotation().Yaw;
	if (ParentYaw > 0.f && ParentYaw - 180.f > LookYawAngle)
	{
		ParentYaw -= 360.f;
	}
	else if(ParentYaw<0.f&&ParentYaw+180.f<LookYawAngle)
	{
		ParentYaw += 360.f;
	}

	LookYawAngle = FMath::Clamp(LookYawAngle, ParentYaw - 20.f, ParentYaw + 20.f);

	Delta = LookYawAngle - Rotator.Yaw + 90.f;

	Gun->SetWorldRotation(FRotator(0., LookYawAngle, -LookRollAngle));
}
