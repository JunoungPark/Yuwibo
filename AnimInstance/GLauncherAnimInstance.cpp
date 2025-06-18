// Fill out your copyright notice in the Description page of Project Settings.


#include "GLauncherAnimInstance.h"
#include "Yuwibo/Character/TankLv3.h"
#include "Camera/CameraActor.h"
#include "TankBackGunAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/GLauncherHUD.h"
#include "Yuwibo/Actor/Projectile/GLauncher.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Yuwibo/Component/MoveComponent.h"

UGLauncherAnimInstance::UGLauncherAnimInstance()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> Montage(TEXT("AnimMontage'/Game/BlueprintClass/Tank/GLauncherAnimMontage.GLauncherAnimMontage'"));
	AttackMontage = Montage.Object;
}
void UGLauncherAnimInstance::Attack(bool IsLookSky, FVector Vector)
{

	if (Montage_IsPlaying(AttackMontage)) return;

	LookSky = IsLookSky;

	TargetLocation = Vector;

	Montage_Play(AttackMontage);

}
void UGLauncherAnimInstance::NativeInitializeAnimation()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Character = static_cast<ATankLv3*>(Pawn);
		GLauncher = GetOwningComponent();
		
	}
	
}
void UGLauncherAnimInstance::AnimNotify_StartPack()
{
	if (Character->IsLocallyControlled()) UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(Character, 0.f);
}

void UGLauncherAnimInstance::AnimNotify_Pack()
{
	//UE_LOG(LogTemp, Log, TEXT("yes"));
	
	auto anim = Character->GetBackSleletalAnim();
	if (anim->GetIsOn())
	{
		anim->SetIsOn(false);
	}
}
void UGLauncherAnimInstance::AnimNotify_UnPack()
{
	UE_LOG(LogTemp, Log, TEXT("FunctionName : OnOffMenu"));

	if (Character->HasAuthority())
	{
		if (!Character->GetComponent<UTankLv3AnimComponent>(Component::Anim)->GetGLauncherMode()) return;

		if(auto Move = Character->GetComponent<UMoveComponent>(Component::Move))
		{
			Move->SetCanAttack(true);
			Move->SetCanMove(true);
		}
	}

	if (Character->IsLocallyControlled()) UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(Character->GetCamera(), 0.2f);

}
void UGLauncherAnimInstance::AnimNotify_GLauncherAttack()
{
	auto AimRotation = Character->GetBaseAimRotation();
	
	Character->GetGLauncher(GLauncher->GetBoneLocation("Barrel_end_1"), LookSky ? AimRotation : UKismetMathLibrary::FindLookAtRotation(GLauncher->GetBoneLocation("Barrel_end_1"), TargetLocation));
	Character->GetGLauncher(GLauncher->GetBoneLocation("Barrel_end_2"), LookSky ? AimRotation : UKismetMathLibrary::FindLookAtRotation(GLauncher->GetBoneLocation("Barrel_end_2"), TargetLocation));

	//UE_LOG(LogTemp, Log, TEXT("yes"));
}