// Fill out your copyright notice in the Description page of Project Settings.


#include "TankLv2.h"
#include "Yuwibo/Component/HealthComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Yuwibo/Animinstance/TankAnimInstance.h"
#include "Yuwibo/Animinstance/TankGunAnimInstance.h"

ATankLv2::ATankLv2()
{

	CharacterName = TEXT("TankLv2");
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetComponent<UHealthComponent>(Component::Health)->SetMaxHealth(150.f);

	auto SpringArm = GetComponent<ULookComponent>(Component::Look)->GetSpringArm();

	SpringArm->SocketOffset = FVector(0., -180., 90.);
	SpringArm->TargetArmLength = 450.f;

	GetMesh()->SetRelativeScale3D(FVector(0.45));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -73.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(78.);
	GetCapsuleComponent()->SetCapsuleRadius(38.);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> MI(TEXT("Material'/Game/Mech_Constructor_Spiders/Materials/M_Spiders_Main_PBR.M_Spiders_Main_PBR'"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> STM(TEXT("StaticMesh'/Game/Mech_Constructor_Spiders/Meshes/Cockpit_Gun_2x.Cockpit_Gun_2x'"));
	if (STM.Succeeded())
	{
		StaticMesh->SetStaticMesh(STM.Object);
	}
	
	SecondStaticMesh->SetupAttachment(StaticMesh, TEXT("Mount_Weapon_L"));

	ThirdStaticMesh->SetupAttachment(StaticMesh, TEXT("Mount_Weapon_R"));


	ConstructorHelpers::FObjectFinder<USkeletalMesh> SSM(TEXT("SkeletalMesh'/Game/Mech_Constructor_Spiders/Meshes_Skeletal/Weapons/Weapon_Double_Gun_Lvl3.Weapon_Double_Gun_Lvl3'"));
	if (SSM.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(SSM.Object);
		SkeletalMesh->SetupAttachment(SecondStaticMesh, TEXT("Mount_Weapon"));
		
		SecondSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon_Double_Gun_Lvl3"));

		SecondSkeletalMesh->SetSkeletalMesh(SSM.Object);
		SecondSkeletalMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		SecondSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		SecondSkeletalMesh->SetupAttachment(ThirdStaticMesh, TEXT("Mount_Weapon"));

		ConstructorHelpers::FClassFinder<UTankGunAnimInstance> GUN(TEXT("AnimBlueprint'/Game/BlueprintClass/Tank/TankGunAB.TankGunAB_C'"));
		SecondSkeletalMesh->SetAnimInstanceClass(GUN.Class);
		if (MI.Succeeded())
		{
			SecondSkeletalMesh->SetMaterial(0, MI.Object);
		}
		SecondSkeletalMesh->SetIsReplicated(true);
	}

	if (auto Health = GetComponent<UHealthComponent>(Component::Health))
	{
		Health->AddMesh(SecondSkeletalMesh);
	}

	//ConstructorHelpers::FObjectFinder<USoundWave> SW(TEXT("SoundWave'/Game/UltimateSFXBundle/wavs/RealGuns2/Stereo/Weapons/AntiTankGun/antitank_shTL_single_02.antitank_shTL_single_02'"));
	//if (SW.Succeeded())
	//{
	//	GunAudio = CreateDefaultSubobject<UAudioComponent>("GunAudio");
	//	GunAudio->SetSound(SW.Object);
	//	GunAudio->SetupAttachment(SkeletalMesh, TEXT("Barrel_Base"));
	//
	//}

}

void ATankLv2::BeginPlay()
{
	Super::BeginPlay();
	
	static_cast<UTankAnimInstance*>(GetMesh()->GetAnimInstance())->OnOffAttack.AddUFunction(SecondSkeletalMesh->GetAnimInstance(), FName("SetOnOffAttack"));

	static_cast<UTankGunAnimInstance*>(SkeletalMesh->GetAnimInstance())->SetLocation(Location::Left);

	static_cast<UTankGunAnimInstance*>(SecondSkeletalMesh->GetAnimInstance())->SetLocation(Location::Right);
}
