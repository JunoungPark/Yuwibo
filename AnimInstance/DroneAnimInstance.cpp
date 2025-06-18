// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAnimInstance.h"
#include "Yuwibo/Character/Drone.h"
#include "Yuwibo/Actor/Projectile/Projectile.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Camera/CameraComponent.h"

void UDroneAnimInstance::AnimNotify_TopGunAttack()
{
	if(IsAttack)
	{
		if (Character->HasAuthority())
		{
			if (Drone->GetController() && Drone->GetController()->IsPlayerController())
			{
				Drone->GetBullet(Drone->GetTopLeftGunLocation(), LookSky ? Drone->GetControlRotation() : UKismetMathLibrary::FindLookAtRotation(Drone->GetTopLeftGunLocation(), TargetLocation));

				Drone->GetBullet(Drone->GetTopRightGunLocation(), LookSky ? Drone->GetControlRotation() : UKismetMathLibrary::FindLookAtRotation(Drone->GetTopRightGunLocation(), TargetLocation));
			}
			else if (Drone->GetComponent<UAiComponent>(Component::AI)->GetTarget())
			{
				Drone->GetBullet(Drone->GetTopLeftGunLocation(), UKismetMathLibrary::FindLookAtRotation(Drone->GetTopLeftGunLocation(), Drone->GetComponent<UAiComponent>(Component::AI)->GetTarget()->GetTargetLocation()));

				Drone->GetBullet(Drone->GetTopRightGunLocation(), UKismetMathLibrary::FindLookAtRotation(Drone->GetTopRightGunLocation(), Drone->GetComponent<UAiComponent>(Component::AI)->GetTarget()->GetTargetLocation()));
			}
		}

		if(!Drone->IsLocallyControlled()) Drone->ResetTopFx();
	}
}

void UDroneAnimInstance::AnimNotify_BotGunAttack()
{
	if (IsAttack)
	{
		if (Character->HasAuthority())
		{
			if (Drone->GetController() && Drone->GetController()->IsPlayerController())
			{
				Drone->GetBullet(Drone->GetBotLeftGunLocation(), LookSky ? Drone->GetControlRotation() : UKismetMathLibrary::FindLookAtRotation(Drone->GetBotLeftGunLocation(), TargetLocation));

				Drone->GetBullet(Drone->GetBotRightGunLocation(), LookSky ? Drone->GetControlRotation() : UKismetMathLibrary::FindLookAtRotation(Drone->GetBotRightGunLocation(), TargetLocation));
			}
			else if(Drone->GetComponent<UAiComponent>(Component::AI)->GetTarget())
			{
				Drone->GetBullet(Drone->GetBotLeftGunLocation(), UKismetMathLibrary::FindLookAtRotation(Drone->GetBotLeftGunLocation(), Drone->GetComponent<UAiComponent>(Component::AI)->GetTarget()->GetTargetLocation()));

				Drone->GetBullet(Drone->GetBotRightGunLocation(), UKismetMathLibrary::FindLookAtRotation(Drone->GetBotRightGunLocation(), Drone->GetComponent<UAiComponent>(Component::AI)->GetTarget()->GetTargetLocation()));
			}
		}

		if (!Drone->IsLocallyControlled()) Drone->ResetBotFx();
	}
}

void UDroneAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (Character)
	{
		Drone = Cast<ADrone>(Character);
	}
}

//void UDroneAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	Super::NativeUpdateAnimation(DeltaSeconds);
//
//	if (!Drone) return;
//
//	if (!Drone->HasAuthority() && !Drone->IsLocallyControlled()) return;
//
//	if (Drone->GetController() && Drone->GetController()->IsPlayerController())
//	{
//
//		auto ComponentLocation = Drone->GetComponent<ULookComponent>(Component::Look)->GetCamera()->GetComponentLocation();
//
//		FCollisionQueryParams Params(NAME_None, false, Drone);
//		Params.bIgnoreTouches = true;
//		Params.AddIgnoredActor(Drone);
//
//		FHitResult HitResult;
//		LookSky = !GetWorld()->LineTraceSingleByChannel(HitResult, ComponentLocation, ComponentLocation + Drone->GetControlRotation().Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params);
//
//		TargetLocation = HitResult.ImpactPoint;
//
//		auto Direct = (TargetLocation - Drone->GetActorLocation()).GetSafeNormal();
//		FRotator LookAtRotation = FRotator(FMath::RadiansToDegrees(FMath::Atan2(Direct.Z, FVector(Direct.X, Direct.Y, 0).Size())), Drone->GetControlRotation().Yaw, 0.0f);
//
//		Drone->SetActorRotation(!LookSky ? LookAtRotation : Drone->GetControlRotation());
//	}
//}
