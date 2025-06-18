// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainerAnimInstance.h"
#include "Yuwibo/Actor/Projectile/Arrow.h"
#include "Yuwibo/Character/Trainer.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "MooseAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Yuwibo/Component/LookComponent.h"
void UTrainerAnimInstance::AnimNotify_CatchArrow()
{
	//if (Character->HasAuthority()) UE_LOG(LogTemp, Log, TEXT("CatchArrow"));
}

void UTrainerAnimInstance::AnimNotify_CatchString()
{
	//Trainer->GetArrow()->SetVisibility(true);
	//Trainer->SetCatchString(true);
	if (!Character->HasAuthority()) 
	{
		UE_LOG(LogTemp, Log, TEXT("CatchString"));
	}
	else 
		Trainer->EndAttack();

}

void UTrainerAnimInstance::AnimNotify_PutString()
{
	
	//Trainer->SetCatchString(false);
	//Trainer->GetArrow()->SetVisibility(false);

	if (!Character->HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("PutString"));
		return;
	}

	FVector AttackHand = Trainer->GetWeapon()->GetSocketLocation("bow_stringSocket");
	FRotator AttackRotation = LookSky ? AimRotation : UKismetMathLibrary::FindLookAtRotation(AttackHand, AttackHitResult);

	Trainer->GetArrow(AttackHand + AttackRotation.Vector() * 30, AttackRotation);
	
	//Trainer->PlayMagicSound();
}

void UTrainerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (Character)
	{
		Trainer = Cast<ATrainer>(Character);
	}
}

//void UTrainerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	Super::NativeUpdateAnimation(DeltaSeconds);
//	if (Trainer)
//	{
//		FCollisionQueryParams Params(NAME_None, false, Trainer);
//		Params.bIgnoreTouches = true;
//		Params.AddIgnoredActor(Trainer);
//
//		FRotator Target;
//		if (RideMoose)
//		{
//			Params.AddIgnoredActor(RideMoose);
//
//			//Trainer->SetActorLocation(RideMoose->GetMesh()->GetSocketLocation("spine_2Socket"));
//			FRotator MooseRotator = RideMoose->GetMesh()->GetSocketRotation("spine_2Socket");
//			//RideMoose->SetActorRotation(FRotator(TrainerRotator.Roll, RideMoose->GetActorRotation().Yaw, 0.));
//
//			//Trainer->SetActorRotation(FRotator(Trainer->GetActorRotation().Pitch, MooseRotator.Yaw, Trainer->GetActorRotation().Roll));
//			
//
//			//MovingRotation = Character->GetActorRotation();
//			//FinalRotation = MovingRotation;
//			
//			//RideMoose->MoveUpDown(ForwardValue);
//			
//			
//			//RideMoose->MoveLeftRight(RightValue);
//				
//			UMooseAnimInstance* MooseAnim = Cast<UMooseAnimInstance>(RideMoose->GetMesh()->GetAnimInstance());
//			MooseBlend = MooseAnim->GetBlendAlpha();
//			MooseSpeed = MooseAnim->GetGroundSpeed();
//			RootYaw = MooseRotator.Yaw - Trainer->GetActorRotation().Yaw;
//
//		}
//
//		ULookComponent* Look = Trainer->GetComponent<ULookComponent>(Component::Look);
//
//		FHitResult HitResult;
//		
//		if (GetWorld()->LineTraceSingleByChannel(HitResult, Look->GetCamera()->GetComponentLocation(), Look->GetCamera()->GetComponentLocation() + AimRotation.Vector() * 50000.f, ECollisionChannel::ECC_Visibility, Params))
//		{
//			AttackHitResult = HitResult.ImpactPoint;
//			//DrawDebugLine(GetWorld(), Character->GetCamera()->GetComponentLocation(), HitResult.ImpactPoint, FColor::Green, false, 2.f);
//			Target = UKismetMathLibrary::FindLookAtRotation(Trainer->GetActorLocation(), AttackHitResult);
//
//			Target.Pitch += - Trainer->GetActorRotation().Pitch;
//			Target.Yaw += -Trainer->GetActorRotation().Yaw - RootYaw;
//		
//			LookSky = false;
//		}
//		else
//		{
//			Target.Pitch = AimRotation.Pitch - Trainer->GetActorRotation().Pitch;
//			Target.Yaw = AimRotation.Yaw - Trainer->GetActorRotation().Yaw - RootYaw;
//			LookSky = true;
//		}
//
//		//
//		while (Target.Yaw < -180)
//		{
//			Target.Yaw += 360;
//		}
//		while (Target.Yaw > 180)
//		{
//
//			Target.Yaw -= 360;
//		}
//		BlendRotator.Pitch = FMath::FInterpTo(BlendRotator.Pitch, Target.Pitch, DeltaSeconds, 10.);
//		BlendRotator.Yaw = FMath::FInterpTo(BlendRotator.Yaw, FMath::Clamp(Target.Yaw, -90, 90), DeltaSeconds, 10.);
//		
//	}
//}

void UTrainerAnimInstance::Ride(AMoose* Moose)
{
	ULookComponent* Look = Trainer->GetComponent<ULookComponent>(Component::Look);
	
	if (Moose)
	{
		IsRide = true;
		RideMoose = Moose;
		Trainer->GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
		Look->GetSpringArm()->bDoCollisionTest = false;
	}
	else
	{
		IsRide = false;
		RideMoose = nullptr;
		Trainer->GetCapsuleComponent()->SetCollisionProfileName("Pawn");
		Look->GetSpringArm()->bDoCollisionTest = true;
	}
	
}
