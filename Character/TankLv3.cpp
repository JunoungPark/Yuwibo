// Fill out your copyright notice in the Description page of Project Settings.


#include "TankLv3.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Component/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Yuwibo/Animinstance/TankGunAnimInstance.h"
#include "Yuwibo/Animinstance/TankAnimInstance.h"
#include "Yuwibo/Animinstance/GLauncherAnimInstance.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Net/UnrealNetwork.h"
#include "Yuwibo/Actor/Projectile/ProjectilePool.h"
#include "Yuwibo/Actor/Projectile/GLauncher.h"
ATankLv3::ATankLv3()
{
	CharacterName = TEXT("TankLv3");

	PrimaryActorTick.bCanEverTick = true;

	GetComponent<UHealthComponent>(Component::Health)->SetMaxHealth(200.f);

	auto SpringArm = GetComponent<ULookComponent>(Component::Look)->GetSpringArm();
	
	SpringArm->SocketOffset = FVector(0., -400., 250.);
	SpringArm->TargetArmLength = 1000.f;

	GLauncherProjectilePool = CreateDefaultSubobject<UProjectilePool>(FName("GLauncherProjectilePool"));

	GetMesh()->SetRelativeScale3D(FVector(1.));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -162.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(174.);
	GetCapsuleComponent()->SetCapsuleRadius(85.);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> MI(TEXT("Material'/Game/Mech_Constructor_Spiders/Materials/M_Spiders_Main_PBR.M_Spiders_Main_PBR'"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> STM(TEXT("StaticMesh'/Game/Mech_Constructor_Spiders/Meshes/Cockpit_Gun_Flat.Cockpit_Gun_Flat'"));
	if (STM.Succeeded())
	{
		StaticMesh->SetStaticMesh(STM.Object);
	}

	SecondStaticMesh->SetupAttachment(StaticMesh, TEXT("Mount_HalfShoulder_L"));

	ThirdStaticMesh->SetupAttachment(StaticMesh, TEXT("Mount_HalfShoulder_R"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Mech_Constructor_Spiders/Meshes_Skeletal/Backpack_Gun_Mount.Backpack_Gun_Mount'"));
	if (SM.Succeeded())
	{
		BackSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpack_Gun_Mount")); 
		BackSkeletalMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		BackSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		BackSkeletalMesh->SetSkeletalMesh(SM.Object); 
		BackSkeletalMesh->SetupAttachment(StaticMesh, TEXT("Mount_Backpack"));

		ConstructorHelpers::FClassFinder<UTankBackGunAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Tank/BackGunAnimBlueprint.BackGunAnimBlueprint_C'"));
		if (AB.Succeeded())
		{
			BackSkeletalMesh->SetAnimInstanceClass(AB.Class);
		}
		if (MI.Succeeded())
		{
			BackSkeletalMesh->SetMaterial(0, MI.Object);
		}

		ConstructorHelpers::FObjectFinder<UStaticMesh> FSTM(TEXT("StaticMesh'/Game/Mech_Constructor_Spiders/Meshes/Cockpit_Gun_2x.Cockpit_Gun_2x'"));
		if (FSTM.Succeeded())
		{
			FourthStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cockpit_Gun_2x"));
			FourthStaticMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
			FourthStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
			FourthStaticMesh->SetStaticMesh(FSTM.Object);
			FourthStaticMesh->SetupAttachment(BackSkeletalMesh, TEXT("Mount_Weapon_Top"));
			if (MI.Succeeded())
			{
				FourthStaticMesh->SetMaterial(0, MI.Object);
			}

			ConstructorHelpers::FObjectFinder<USkeletalMesh> GLM(TEXT("SkeletalMesh'/Game/Mech_Constructor_Spiders/Meshes_Skeletal/Weapons/Weapon_GLauncher_Lvl3.Weapon_GLauncher_Lvl3'"));
			if (GLM.Succeeded())
			{

				FourthSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("L_Weapon_GLauncher_Lvl3"));
				FourthSkeletalMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
				FourthSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
				FourthSkeletalMesh->SetSkeletalMesh(GLM.Object);
				FourthSkeletalMesh->SetupAttachment(FourthStaticMesh, TEXT("Mount_Weapon_L"));
				if (MI.Succeeded())
				{
					FourthSkeletalMesh->SetMaterial(0, MI.Object);
				}

				FifthSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("R_Weapon_GLauncher_Lvl3"));
				FifthSkeletalMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
				FifthSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
				FifthSkeletalMesh->SetSkeletalMesh(GLM.Object);
				FifthSkeletalMesh->SetupAttachment(FourthStaticMesh, TEXT("Mount_Weapon_R"));
				if (MI.Succeeded())
				{
					FifthSkeletalMesh->SetMaterial(0, MI.Object);
				}

				ConstructorHelpers::FClassFinder<UGLauncherAnimInstance> GAB(TEXT("AnimBlueprint'/Game/BlueprintClass/Tank/GLauncherAnimBlueprint.GLauncherAnimBlueprint_C'"));
				if (GAB.Succeeded())
				{
					FourthSkeletalMesh->SetAnimInstanceClass(GAB.Class);
					FifthSkeletalMesh->SetAnimInstanceClass(GAB.Class);

				}
			}
		}
	}

	



	ConstructorHelpers::FObjectFinder<USkeletalMesh> SSM(TEXT("SkeletalMesh'/Game/Mech_Constructor_Spiders/Meshes_Skeletal/Weapons/Weapon_Double_Gun_Lvl5.Weapon_Double_Gun_Lvl5'"));
	if (SSM.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(SSM.Object);
		//SkeletalMesh->SetupAttachment(SecondStaticMesh, TEXT("Mount_Weapon"));

		SecondSkeletalMesh->SetSkeletalMesh(SSM.Object);
		//SecondSkeletalMesh->SetupAttachment(ThirdStaticMesh, TEXT("Mount_Weapon"));

		ThirdSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon_Double_Gun_Lvl5")); 
		ThirdSkeletalMesh->SetSkeletalMesh(SSM.Object);
		ThirdSkeletalMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		ThirdSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		ThirdSkeletalMesh->SetupAttachment(StaticMesh, TEXT("Mount_Weapon_Main"));

		ConstructorHelpers::FClassFinder<UTankGunAnimInstance> GUN(TEXT("AnimBlueprint'/Game/BlueprintClass/Tank/TankGunAB.TankGunAB_C'"));
		ThirdSkeletalMesh->SetAnimInstanceClass(GUN.Class);

		if (MI.Succeeded())
		{
			ThirdSkeletalMesh->SetMaterial(0, MI.Object);
		}
	}


	if (auto Health = GetComponent<UHealthComponent>(Component::Health))
	{
		Health->AddMesh(ThirdSkeletalMesh);
		Health->AddMesh(BackSkeletalMesh);
		Health->AddMesh(FourthStaticMesh);
		Health->AddMesh(FourthSkeletalMesh);
		Health->AddMesh(FifthSkeletalMesh);
	}

	ThirdSkeletalMesh->SetIsReplicated(true);
	BackSkeletalMesh->SetIsReplicated(true);
	FourthStaticMesh->SetIsReplicated(true);
	FourthSkeletalMesh->SetIsReplicated(true);
	FifthSkeletalMesh->SetIsReplicated(true);
	
}

void ATankLv3::WeaponChange_Implementation()
{
	if (UMoveComponent* Move = GetComponent<UMoveComponent>(Component::Move))
	{
		if (Move->GetCanAttack() && GetCharacterMovement()->IsFalling() == false)
		{
			if (auto Anim = GetComponent<UTankLv3AnimComponent>(Component::Anim))
			{
				Move->SetCanAttack(false);

				Anim->ChangeGLauncherMode();

				if (Anim->GetGLauncherMode())
				{
					if (TankMode) Jump();
					else Move->SetCanMove(false);

					static_cast<UTankBackGunAnimInstance*>(BackSkeletalMesh->GetAnimInstance())->SetIsOn(true);
				}
				else
				{
					Move->SetCanMove(false);
					
					if (auto FourthAnim = static_cast<UGLauncherAnimInstance*>(FourthSkeletalMesh->GetAnimInstance()))
					{
						FourthAnim->Montage_Stop(0.f);

						FourthAnim->SetIsPack(true);
					}
					if(auto FifthAnim = static_cast<UGLauncherAnimInstance*>(FifthSkeletalMesh->GetAnimInstance()))
					{
						FifthAnim->Montage_Stop(0.f);

						FifthAnim->SetIsPack(true);
					}
				}

				ChangeWeapon(Anim->GetGLauncherMode());
			}
		}
	}
}

void ATankLv3::ChangeWeapon_Implementation(bool IsGLauncherMode)
{
	if (HasAuthority()) return;

	if (IsGLauncherMode) static_cast<UTankBackGunAnimInstance*>(BackSkeletalMesh->GetAnimInstance())->SetIsOn(true);
	else
	{
		if (auto FourthAnim = static_cast<UGLauncherAnimInstance*>(FourthSkeletalMesh->GetAnimInstance()))
		{
			FourthAnim->Montage_Stop(0.f);

			FourthAnim->SetIsPack(true);
		}
		if (auto FifthAnim = static_cast<UGLauncherAnimInstance*>(FifthSkeletalMesh->GetAnimInstance()))
		{
			FifthAnim->Montage_Stop(0.f);

			FifthAnim->SetIsPack(true);
		}
	}
}

void ATankLv3::OnRep_Camera()
{
	GetComponent<ULookComponent>(Component::Look)->GetCamera()->bUsePawnControlRotation = true;
	Camera->GetCameraComponent()->bUsePawnControlRotation = true;
	//Camera->SetActorRotation(FRotator(0., FourthStaticMesh->GetSocketRotation(TEXT("Mount_Weapon_Top")).Yaw + 180., 0.));

	Camera->GetCameraComponent()->bConstrainAspectRatio = false;
	Camera->GetCameraComponent()->bOverrideAspectRatioAxisConstraint = 1;

	//UE_LOG(LogTemp, Log, TEXT("CameraRotation"));
}

void ATankLv3::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATankLv3, Camera);

}

void ATankLv3::BeginPlay()
{
	Super::BeginPlay();

	GLauncherProjectilePool->InitializePool<AGLauncher>(this);

	GetComponent<UMoveComponent>(Component::Move)->OnAttack.AddUFunction(GetMesh()->GetAnimInstance(), FName("GLauncherAttack"));

	static_cast<UTankAnimInstance*>(GetMesh()->GetAnimInstance())->OnOffAttack.AddUFunction(ThirdSkeletalMesh->GetAnimInstance(), FName("SetOnOffAttack"));

	auto TankAnimInstance =static_cast<UTankAnimInstance*>(GetMesh()->GetAnimInstance());
	TankAnimInstance->OnGLauncherAttack.AddUFunction(FourthSkeletalMesh->GetAnimInstance(), FName("Attack"));
	TankAnimInstance->OnGLauncherAttack.AddUFunction(FifthSkeletalMesh->GetAnimInstance(), FName("Attack"));

	auto GunAnim = static_cast<UTankGunAnimInstance*>(ThirdSkeletalMesh->GetAnimInstance());
	GunAnim->SetLocation(Location::Center);

	auto TankBackGunAnimInstance = static_cast<UTankBackGunAnimInstance*>(BackSkeletalMesh->GetAnimInstance());
	TankBackGunAnimInstance->On.AddUFunction(FourthSkeletalMesh->GetAnimInstance(),FName("SetIsPack"));
	TankBackGunAnimInstance->On.AddUFunction(FifthSkeletalMesh->GetAnimInstance(),FName("SetIsPack"));

	if (!HasAuthority()) return;

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	GetComponent<ULookComponent>(Component::Look)->GetCamera()->bUsePawnControlRotation = true;

	Camera = GetWorld()->SpawnActor<ACameraActor>(FourthStaticMesh->GetComponentLocation(), FRotator(0., 0., 0.), spawnParameters);
	Camera->SetReplicates(true);
	Camera->GetCameraComponent()->SetIsReplicated(true);

	Camera->GetCameraComponent()->bUsePawnControlRotation = true;
	Camera->AttachToComponent(FourthStaticMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Mount_Weapon_Top"));
	Camera->SetActorRotation(FRotator(0., FourthStaticMesh->GetSocketRotation(TEXT("Mount_Weapon_Top")).Yaw + 180., 0.));
	Camera->SetActorRelativeLocation(FVector(-48., 135., 0.));
	Camera->GetCameraComponent()->bOverrideAspectRatioAxisConstraint = 1;
}

void ATankLv3::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("SpecialSkill"), EInputEvent::IE_Pressed, this, &ATankLv3::WeaponChange);
}

AProjectile* ATankLv3::GetGLauncher(FVector Location, FRotator Rotation)
{
	return GLauncherProjectilePool->GetProjectile<AGLauncher>(Location, Rotation);
}
