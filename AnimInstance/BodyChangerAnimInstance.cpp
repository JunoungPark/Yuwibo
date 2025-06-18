// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyChangerAnimInstance.h"
#include "Yuwibo/Character/BodyChanger.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Yuwibo/Actor/Projectile/WaterBall.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ShapeComponent.h"
void UBodyChangerAnimInstance::AnimNotify_StartAttack()
{
	if (Character->IsLocallyControlled())
	{
		auto AttackHand = Character->GetMesh()->GetBoneLocation(Montage_GetCurrentSection(AttackMontage) != FName("Attack_1") ? "hand_r" : "hand_l");

		BodyChanger->GetWaterBall(AttackHand, LookSky ? AimRotation : UKismetMathLibrary::FindLookAtRotation(AttackHand, AttackHitResult));
	}

	BodyChanger->PlayMagicSound();
}

void UBodyChangerAnimInstance::AnimNotify_Search()
{
	if (Character->IsLocallyControlled())Character->GetComponent<UBodyChangerAnimComponent>(Component::Anim)->SetAttackRotation();
}

void UBodyChangerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (IsValid(Character))
	{
		BodyChanger = Cast<ABodyChanger>(Character);
	}
}

//void UBodyChangerAnimInstance::FindHUDRotation()
//{
//	ULookComponent* Look = Character->GetComponent<ULookComponent>(Component::Look);
//	
//	FCollisionQueryParams Params(NAME_None, false, Character);
//	Params.bIgnoreTouches = true;
//	FHitResult HitResult;
//	bool Result = GetWorld()->LineTraceSingleByChannel(HitResult, Look->GetCamera()->GetComponentLocation(), Look->GetCamera()->GetComponentLocation() + AimRotation.Vector() * 50000.f, ECollisionChannel::ECC_EngineTraceChannel2, Params);
//	while (Cast<UShapeComponent>(HitResult.Component))
//	{
//		Params.AddIgnoredComponent(HitResult.Component.Get());
//		Result = GetWorld()->LineTraceSingleByChannel(HitResult, Look->GetCamera()->GetComponentLocation(), Look->GetCamera()->GetComponentLocation() + AimRotation.Vector() * 50000.f, ECollisionChannel::ECC_EngineTraceChannel2, Params);
//	}
//	if (Result)
//	{
//		
//		AttackHitResult = HitResult.ImpactPoint;
//		//DrawDebugLine(GetWorld(), Character->GetCamera()->GetComponentLocation(), AttackHitResult, FColor::Green, false, 2.f);
//
//		LookSky = false;
//
//		Character->GetComponent<UAnimComponent>(Component::Anim)->SetAttackYaw(UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), AttackHitResult), AimRotation).Yaw);
//		
//	}
//	else
//	{
//
//		LookSky = true;
//		Character->GetComponent<UAnimComponent>(Component::Anim)->SetAttackYaw();
//	}
//}
