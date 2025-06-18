// Fill out your copyright notice in the Description page of Project Settings.


#include "GolemLv2.h"
#include "Components/CapsuleComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Yuwibo/Animinstance/BaseAnimInstance.h"
#include "Components/AudioComponent.h"
#include "Yuwibo/Animinstance/SoldierAnimInstance.h"
AGolemLv2::AGolemLv2()
{
	CharacterName = "Golem";

	SetComponent<UTwoFootIkComponent>(Component::FootIk)->BindSocket("Foot_L", "Foot_R");

	GetCapsuleComponent()->SetCapsuleHalfHeight(171.f);
	GetCapsuleComponent()->SetCapsuleRadius(80.f);
	GetCapsuleComponent()->SetRelativeScale3D(FVector(3.2));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/EarthGuardian/Meshes/SK_EarthGuardian.SK_EarthGuardian'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -168.f), FRotator(0.f, -90.f, 0.f));
	}
	ConstructorHelpers::FClassFinder<USoldierAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Golem/GolemLV2AB.GolemLV2AB_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}
	ULookComponent* Look = GetComponent<ULookComponent>(Component::Look);

	Look->GetSpringArm()->DestroyComponent();
	Look->GetCamera()->bUsePawnControlRotation = true;
	Look->GetCamera()->SetupAttachment(GetMesh(), "faceAttachSocket");

	ConstructorHelpers::FObjectFinder<USoundWave> SW(TEXT("SoundWave'/Game/Monsters_Footsteps/Wavs/Large_2_Legged/Walk/No_Armor/Hard/Monster_Footsteps_Large_2Legged_NoArmour_Walk_Hard_07_Wav.Monster_Footsteps_Large_2Legged_NoArmour_Walk_Hard_07_Wav'"));
	if (SW.Succeeded())
	{
		FootAudio->SetSound(SW.Object);
		FootAudio->SetupAttachment(GetMesh(), TEXT("root"));
	}
}

//void AGolemLv2::OnHealthUpdate()
//{
//	if (CurrentHealth <= 0)
//	{
//		Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance())->SetIsDead(true);
//
//		FTimerHandle DestoyTimerHandle;
//		GetWorldTimerManager().SetTimer(DestoyTimerHandle, this, &AGolemLv2::Disappear, 3.0f);
//
//	}
//
//	//HpWidget->UpdateHp();
//}