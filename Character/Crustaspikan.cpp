// Fill out your copyright notice in the Description page of Project Settings.


#include "Crustaspikan.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Yuwibo/Animinstance/BaseEnemyAnimInstance.h"
ACrustaspikan::ACrustaspikan()
{
	CharacterName = "Crustaspikan";

	SetComponent<UEnemyComponent>(Component::Enemy);
	SetComponent<UTwoFootIkComponent>(Component::FootIk)->BindSocket("Crustaspikan_-L-Foot", "Crustaspikan_-R-Foot");

	GetCapsuleComponent()->SetCapsuleHalfHeight(545.f);
	GetCapsuleComponent()->SetCapsuleRadius(500.f);

	//SphereComponent->SetSphereRadius(1000.f);

	ConstructorHelpers::FClassFinder<UTwoFootEnemyAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/Crustaspikan/CrustaspikanAB.CrustaspikanAB_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Crustaspikan/Meshes/SK_CrustaspikanBody.SK_CrustaspikanBody'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -544.f), FRotator(0.f, -90.f, 0.f));

	}
	ConstructorHelpers::FObjectFinder<USoundWave> SW(TEXT("SoundWave'/Game/Monsters_Footsteps/Wavs/Large_2_Legged/Walk/No_Armor/Hard/Monster_Footsteps_Large_2Legged_NoArmour_Walk_Hard_07_Wav.Monster_Footsteps_Large_2Legged_NoArmour_Walk_Hard_07_Wav'"));
	if (SW.Succeeded())
	{
		FootAudio->SetSound(SW.Object);
		FootAudio->SetupAttachment(GetMesh(), TEXT("root"));
	}
}

void ACrustaspikan::BeginPlay()
{
	Super::BeginPlay();

	GetComponent<UTwoFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction(GetMesh()->GetAnimInstance(), FName("FootIk"));
}
