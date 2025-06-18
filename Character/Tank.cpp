// Fill out your copyright notice in the Description page of Project Settings.

// Sets default values
#include "Tank.h"
#include "TankLv3.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Yuwibo/Component/HealthComponent.h"
#include "Yuwibo/Component/ItemComponent.h"
#include "Yuwibo/Actor/Projectile/ProjectilePool.h"
#include "Yuwibo/Actor/Projectile/Bullet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Yuwibo/GunHUD.h"
#include "Yuwibo/Animinstance/BaseAnimInstance.h"
#include "Yuwibo/Animinstance/TankAnimInstance.h"
#include "Yuwibo/Animinstance/TankGunAnimInstance.h"
// Sets default values
ATank::ATank()
{
	SetTeamId(FGenericTeamId(100));

	GetComponent<UHealthComponent>(Component::Health)->SetMaxHealth(75.f);

	CharacterName = TEXT("Tank");

	GetComponent<UMoveComponent>(Component::Move)->SetAttackCanMove();
	
	SetComponent<UFourFootIkComponent>(Component::FootIk)->BindSocket("FL_Foot", "FR_Foot", "RL_Foot", "RR_Foot");
	
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetComponent<ULookComponent>(Component::Look)->GetSpringArm()->SocketOffset = FVector(0., -125., 40.);
	
	ProjectilePool = CreateDefaultSubobject<UProjectilePool>(FName("ProjectilePool"));

	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_Yes;
	GetCapsuleComponent()->SetCapsuleHalfHeight(62.);
	GetCapsuleComponent()->SetCapsuleRadius(30.);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> MI(TEXT("Material'/Game/Mech_Constructor_Spiders/Materials/M_Spiders_Main_PBR.M_Spiders_Main_PBR'"));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Mech_Constructor_Spiders/Meshes_Skeletal/Legs_Spiders/Legs_Transformer_Lt.Legs_Transformer_Lt'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		
		GetMesh()->SetRelativeScale3D(FVector(0.35));
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -58.f), FRotator(0.f, -90.f, 0.f));
		
		ConstructorHelpers::FClassFinder<UTankAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Tank/TankAB.TankAB_C'"));

		if (AB.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(AB.Class);
		}
		
		if (MI.Succeeded())
		{
			GetMesh()->SetMaterial(0, MI.Object);
		}
	}

	
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> STM(TEXT("StaticMesh'/Game/Mech_Constructor_Spiders/Meshes/Cockpit_Gun_Flat.Cockpit_Gun_Flat'"));
	if (STM.Succeeded())
	{
		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun_Flat"));

		StaticMesh->SetStaticMesh(STM.Object);
		StaticMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		StaticMesh->SetupAttachment(GetMesh(),TEXT("Mount_Top"));

		if (MI.Succeeded())
		{
			StaticMesh->SetMaterial(0, MI.Object);
		}
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> SSTM(TEXT("StaticMesh'/Game/Mech_Constructor_Spiders/Meshes/HalfShoulder_Bot.HalfShoulder_Bot'"));
	if (SSTM.Succeeded())
	{
		SecondStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HalfShoulder_Bot"));
		ThirdStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HalfShoulder_Bot2"));


		SecondStaticMesh->SetStaticMesh(SSTM.Object);
		SecondStaticMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		SecondStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		SecondStaticMesh->SetupAttachment(StaticMesh, TEXT("Mount_HalfShoulder_L"));

		ThirdStaticMesh->SetStaticMesh(SSTM.Object);
		ThirdStaticMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		ThirdStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		ThirdStaticMesh->SetupAttachment(StaticMesh, TEXT("Mount_HalfShoulder_R"));

		if (MI.Succeeded())
		{
			SecondStaticMesh->SetMaterial(0, MI.Object);
		}

		if (MI.Succeeded())
		{
			ThirdStaticMesh->SetMaterial(0, MI.Object);
		}
	}

	
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SSM(TEXT("SkeletalMesh'/Game/Mech_Constructor_Spiders/Meshes_Skeletal/Weapons/Weapon_Double_Gun_Lvl1.Weapon_Double_Gun_Lvl1'"));
	if (SSM.Succeeded())
	{
		SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon_Double_Gun_Lvl1")); 
		
		SkeletalMesh->SetSkeletalMesh(SSM.Object);
		SkeletalMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		SkeletalMesh->SetupAttachment(StaticMesh,TEXT("Mount_Weapon_Main"));

		ConstructorHelpers::FClassFinder<UTankGunAnimInstance> GUN(TEXT("AnimBlueprint'/Game/BlueprintClass/Tank/TankGunAB.TankGunAB_C'"));
		SkeletalMesh->SetAnimInstanceClass(GUN.Class);
		if (MI.Succeeded())
		{
			SkeletalMesh->SetMaterial(0, MI.Object);
		}
	}

	if (auto Health = GetComponent<UHealthComponent>(Component::Health))
	{
		Health->AddMesh(SkeletalMesh);
		Health->AddMesh(StaticMesh);
		Health->AddMesh(SecondStaticMesh);
		Health->AddMesh(ThirdStaticMesh);
	}

	SkeletalMesh->SetIsReplicated(true);
	StaticMesh->SetIsReplicated(true);
	SecondStaticMesh->SetIsReplicated(true);
	ThirdStaticMesh->SetIsReplicated(true);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS(TEXT("NiagaraSystem'/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_AR_Muzzle_flashes_01.NS_AR_Muzzle_flashes_01'"));
	if (NS.Succeeded())
	{
		MuzzleFx = NS.Object;
		LeftMuzzleFx = CreateDefaultSubobject<UNiagaraComponent>("LeftMuzzleFx");
		LeftMuzzleFx->SetAsset(MuzzleFx);
		LeftMuzzleFx->SetupAttachment(SkeletalMesh, TEXT("Barrel_End_2"));

		RightMuzzleFx = CreateDefaultSubobject<UNiagaraComponent>("RightMuzzleFx");
		RightMuzzleFx->SetAsset(MuzzleFx);
		RightMuzzleFx->SetupAttachment(SkeletalMesh, TEXT("Barrel_End_1"));
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> SNS(TEXT("NiagaraSystem'/Game/Gun_VFX/Niagara_FX/Misc_Gun_FX/NS_AR_Over_heating_smoke.NS_AR_Over_heating_smoke'"));
	if (SNS.Succeeded())
	{
		OverHeatFx = SNS.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave> SW(TEXT("SoundWave'/Game/UltimateSFXBundle/wavs/RealGuns2/Stereo/Weapons/AntiTankGun/antitank_shTL_single_02.antitank_shTL_single_02'"));
	if (SW.Succeeded())
	{
		GunAudio = CreateDefaultSubobject<UAudioComponent>("GunAudio");
		GunAudio->SetSound(SW.Object);
		GunAudio->SetupAttachment(SkeletalMesh,TEXT("Barrel_Base"));
	
	}

	ConstructorHelpers::FObjectFinder<USoundWave> FSW(TEXT("SoundWave'/Game/TWSFX_FS_Ultimate1/Metal2/WAV/WAV_TomWinandySFX_-_FS_Metal2_land_02.WAV_TomWinandySFX_-_FS_Metal2_land_02'"));
	if (FSW.Succeeded())
	{
		SpiderSound = FSW.Object;
		FootAudio->SetSound(SpiderSound);
		FootAudio->SetupAttachment(StaticMesh, TEXT("ROOT"));
	}
	ConstructorHelpers::FObjectFinder<USoundWave> SFSW(TEXT("SoundWave'/Game/UltimateSFXBundle/wavs/CarEngines1/City/car_city_f_engine_loop_1x.car_city_f_engine_loop_1x'"));
	if (SFSW.Succeeded())
	{
		MoveRollSound = SFSW.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave> TFSW(TEXT("SoundWave'/Game/UltimateSFXBundle/wavs/CarEngines1/City/car_city_f_engine_loop_0x.car_city_f_engine_loop_0x'"));
	if (TFSW.Succeeded())
	{
		StayRollSound = TFSW.Object;
	}

	TankMode = false;

}

//void ATank::OnHealthUpdate()
//{
//	if (CurrentHealth <= 0)
//	{
//		Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance())->SetIsDead(true);
//
//		FTimerHandle DestoyTimerHandle;
//		GetWorldTimerManager().SetTimer(DestoyTimerHandle, this, &ATank::Disappear, 3.0f);
//
//	}
//	//HpWidget->UpdateHp();
//}

void ATank::Upgrade_Implementation()
{
	if (Cast<ATankLv3>(this) || GetComponent<UHealthComponent>(Component::Health)->IsDissolving()) return;

	if(auto Item = GetComponent<UItemComponent>(Component::Item))
	{
		if (!Item->CheckItemTypeCount(ItemType::Iron, IsA(ATankLv2::StaticClass()) ? 20 : 10)) return;

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FActorSpawnParameters spawnParameters;
		spawnParameters.Instigator = GetInstigator();
		spawnParameters.Owner = this;

		if (auto Tank = Cast<ATankLv2>(this) ?GetWorld()->SpawnActor<ATankLv3>(GetActorLocation(), GetActorRotation(), spawnParameters) : GetWorld()->SpawnActor<ATankLv2>(GetActorLocation(), GetActorRotation(), spawnParameters))
		{
			if (auto TankHealth = Tank->GetComponent<UHealthComponent>(Component::Health))
			{

				TankHealth->Dissolving();

				Tank->Visible(false);

				if (auto Move = Tank->GetComponent<UMoveComponent>(Component::Move))
				{
					Move->SetAttack<false>();

					Move->SetCanAttack(false);

					Move->SetCanMove(false);
				}

				GetCharacterMovement()->SetMovementMode(MOVE_None);

				GetCharacterMovement()->StopMovementImmediately();

				if(auto Move = GetComponent<UMoveComponent>(Component::Move))
				{
					Move->SetAttack<false>();

					Move->SetCanAttack(false);

					Move->SetCanMove(false);
				}

				auto Rotation = StaticMesh->GetComponentRotation();

				Tank->SetLookGun(Rotation.Yaw, Rotation.Roll);

				//Tank->MoveUpdate(GetActorLocation(), GetActorRotation());

				if (auto Health = GetComponent<UHealthComponent>(Component::Health))
				{
					UGameplayStatics::ApplyDamage(Tank, Health->GetReducedHealth(), GetInstigator()->Controller, this, UDamageType::StaticClass());

					Health->StartDissolve();

					Health->DissolveEnd.AddUFunction(TankHealth, FName("StartDissolveOff"));

					NextBody = Tank;

					Health->DissolveEnd.AddUFunction(this, FName("UpgradePossess"));
				}
				
			}
		}
		else
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			return;
		}

		Item->RemoveItemTypeCount(ItemType::Iron, IsA(ATankLv2::StaticClass()) ? 20 : 10);

		NextBody->GetComponent<UItemComponent>(Component::Item)->CopyItem(Item->GetItem());

	}
}
void ATank::UpgradePossess_Implementation()
{
	if (NextBody)
	{
		GetController()->Possess(NextBody);
		NextBody->Visible(true);
	}
}

void ATank::Visible_Implementation(bool IsTrue)
{
	if (HasAuthority()) return;

	for (auto& TankMesh : GetComponent<UHealthComponent>(Component::Health)->GetMesh())
	{	
		TankMesh->SetCollisionEnabled(IsTrue ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	GetMesh()->SetCollisionEnabled(IsTrue ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(IsTrue ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}
void ATank::Disable_Implementation(bool IsTrue)
{
	IsTrue ? DisableInput(GetController<APlayerController>()) : EnableInput(GetController<APlayerController>());
}
void ATank::BeginPlay()
{
	Super::Super::BeginPlay();
	

	auto Move = GetComponent<UMoveComponent>(Component::Move);

	//Move->OnOffAttack.AddUFunction(GetMesh()->GetAnimInstance(), FName("SetIsAttack"));
	Move->OnOffAttack.AddUFunction(this, FName("OnOffAttack"));

	if (auto Anim = GetComponent<UAnimComponent>(Component::Anim))
	{
		Move->OnOffAttack.AddUFunction(Anim, FName("SetIsAttack"));
	}

	ProjectilePool->InitializePool<ABullet>(this);

	static_cast<UTankAnimInstance*>(GetMesh()->GetAnimInstance())->OnOffAttack.AddUFunction(SkeletalMesh->GetAnimInstance(), FName("SetOnOffAttack"));
	
	auto GunAnim = static_cast<UTankGunAnimInstance*>(SkeletalMesh->GetAnimInstance());
	GunAnim->SetLocation(Location::Center);

	FootAudio->Deactivate();

	GetComponent<UFourFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction((GetMesh()->GetAnimInstance()), FName("FootIk"));

	//UGameplayStatics::GetPlayerController(this, 0)->ClientSetHUD(AGunHUD::StaticClass());
	LeftMuzzleFx->Deactivate();
	RightMuzzleFx->Deactivate();
	GunAudio->Deactivate();

	HasAuthority() ? Move->OnOffAttack.AddUFunction(this, FName("IsServer")) : Move->OnOffAttack.AddUFunction(this, FName("IsClient"));

}

void ATank::MoveUpdate_Implementation(FVector vector, FRotator rotation)
{
	if (HasAuthority()) return;

	SetActorLocationAndRotation(vector, rotation);
	
}

AProjectile* ATank::GetBullet(FVector Location, FRotator Rotation)
{
	return ProjectilePool->GetProjectile<ABullet>(Location, Rotation);
}

void ATank::ResetLeftFx()
{
	if (!LeftMuzzleFx->IsActive())
	{
		LeftMuzzleFx->Activate();
	}
	else
	{
		LeftMuzzleFx->ResetSystem();
	}

	if (LeftMuzzleFx->GetAsset() == MuzzleFx)
	{
		isHeatLeftMuzzle = true;
	}
}

void ATank::ResetRightFx()
{
	if (!RightMuzzleFx->IsActive())
	{
		RightMuzzleFx->Activate();
	}
	else
	{
		RightMuzzleFx->ResetSystem();
	}

	if (RightMuzzleFx->GetAsset() == MuzzleFx)
	{
		isHeatRightMuzzle = true;
	}
}

void ATank::PlayGunSound()
{
	if (!GunAudio->IsActive())
	{
		GunAudio->Activate();
	}
	else
	{
		GunAudio->Play();
	}
}

void ATank::OnOffShake(bool IsTrue)
{
	if (IsTrue)
	{
		IsShake++;
		if (TankMode && IsShake == 1)
			UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(CameraShake);
	}
	else
	{
		IsShake--;
		if (TankMode && IsShake == 0)
			UGameplayStatics::GetPlayerController(this, 0)->ClientStopCameraShake(CameraShake);
	}
}

void ATank::Alram_Implementation(float Delta)
{
	UE_LOG(LogTemp, Log, TEXT("%f"), Delta);
}


int ATank::GetRollSound()
{
	if (FootAudio->GetSound() == StayRollSound)
		return 0;
	else if (FootAudio->GetSound() == MoveRollSound)
		return 1;
	return -1;
}

void ATank::SetRollSound()
{
	if (FootAudio->GetSound() == StayRollSound)
		FootAudio->SetSound(MoveRollSound);
	else
		FootAudio->SetSound(StayRollSound);
}


void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, GetComponent<UMoveComponent>(Component::Move), &UMoveComponent::SetAttack<false>);
	PlayerInputComponent->BindAction(TEXT("SpecialSkill"), EInputEvent::IE_Pressed, this, &ATank::Upgrade);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	//if (HasAuthority())MoveUpdate(GetActorLocation(), GetActorRotation());
}

void ATank::OnOffAttack(bool IsTrue)
{
	if (IsTrue)
	{
		isHeatLeftMuzzle = false;
		isHeatRightMuzzle = false;
		if (LeftMuzzleFx->GetAsset() != MuzzleFx)
		{
			LeftMuzzleFx->SetAsset(MuzzleFx);
		}
		if (RightMuzzleFx->GetAsset() != MuzzleFx)
		{
			RightMuzzleFx->SetAsset(MuzzleFx);
		}
	}
	else
	{
		if (isHeatLeftMuzzle)
		{
			LeftMuzzleFx->SetAsset(OverHeatFx);
		}
		if (isHeatRightMuzzle)
		{
			RightMuzzleFx->SetAsset(OverHeatFx);
		}
	}
}

void ATank::Jump()
{
	ModeChange();
}
void  ATank::ModeChange_Implementation()
{
	if (auto Anim = Cast<UTankLv3AnimComponent>(GetComponent<UTankAnimComponent>(Component::Anim)))
	{
		if (Anim->GetGLauncherMode() && GetComponent<UMoveComponent>(Component::Move)->GetCanAttack()) return;
	}

	if (GetComponent<UMoveComponent>(Component::Move)->GetCanMove() && GetCharacterMovement()->IsFalling() == false)
	{
		if (UMoveComponent* Move = GetComponent<UMoveComponent>(Component::Move))
			Move->SetCanMove(false);

		TankMode = !TankMode;
		if (!TankMode)
		{
			GetComponent<UFourFootIkComponent>(Component::FootIk)->BindSocket("FL_Foot", "FR_Foot", "RL_Foot", "RR_Foot");
			GetCharacterMovement()->MaxWalkSpeed = 400.f;
		}
		else
		{
			GetComponent<UFourFootIkComponent>(Component::FootIk)->BindSocket("FL_Wheel", "FR_Wheel", "RL_Wheel", "RR_Wheel");
			GetCharacterMovement()->MaxWalkSpeed = 1900.f;
		}

		ChangeMode();
	}
}

void ATank::ChangeMode_Implementation()
{
	if (HasAuthority()) return;

	TankMode = !TankMode;
	if (!TankMode)
	{
		if (IsLocallyControlled() && IsShake > 0)
			UGameplayStatics::GetPlayerController(this, 0)->ClientStopCameraShake(CameraShake);

		FootAudio->SetSound(SpiderSound);
		FootAudio->Deactivate();

		GetComponent<UFourFootIkComponent>(Component::FootIk)->BindSocket("FL_Foot", "FR_Foot", "RL_Foot", "RR_Foot");
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
	else
	{
		if (IsLocallyControlled() && IsShake > 0)
			UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(CameraShake);

		GetComponent<UFourFootIkComponent>(Component::FootIk)->BindSocket("FL_Wheel", "FR_Wheel", "RL_Wheel", "RR_Wheel");
		GetCharacterMovement()->MaxWalkSpeed = 1900.f;
	}
	
}
