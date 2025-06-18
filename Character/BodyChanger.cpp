// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyChanger.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Component/SkillComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/GunHUD.h"
#include "LandscapeStreamingProxy.h"
#include "Yuwibo/Animinstance/BodyChangerAnimInstance.h"
#include "Yuwibo/Animinstance/BaseEnemyAnimInstance.h"
#include "Yuwibo/Actor/Projectile/ProjectilePool.h"
#include "Yuwibo/Actor/Projectile/WaterBall.h"
#include "Yuwibo/Component/ItemComponent.h"
#include "Yuwibo/Component/HealthComponent.h"
ABodyChanger::ABodyChanger()
{
	SetTeamId(FGenericTeamId(100));

	GetComponent<UHealthComponent>(Component::Health)->SetMaxHealth(300.f);
	
	CharacterName = TEXT("BodyChanger");

	SetComponent<UTwoFootIkComponent>(Component::FootIk)->BindSocket("foot_l", "foot_r");

	SetComponent<USkillComponent>(Component::Skill)->SetCoolTime(2.);

	SetComponent<UEquipmentComponent>(Component::Equipment);

	ProjectilePool = CreateDefaultSubobject<UProjectilePool>(FName("ProjectilePool"));

	ConstructorHelpers::FClassFinder<UBodyChangerAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/BodyChanger/BodyChangerAnimBlueprint.BodyChangerAnimBlueprint_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);


	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Wizard/Mesh/Wizard.Wizard'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f,0.f,-89.f),FRotator(0.f,-90.f,0.f));


	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_magicCircle1.P_ky_magicCircle1'"));
	if (PS.Succeeded())
	{
		TeleportParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("TeleportParticle");
		TeleportParticleComponent->SetTemplate(PS.Object);
		TeleportParticleComponent->SetRelativeScale3D(FVector(0.5));
		TeleportParticleComponent->SetIsReplicated(true);
		TeleportParticleComponent->SetAutoActivate(false);

		SecondTeleportParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("SecondTeleportParticle");
		SecondTeleportParticleComponent->SetTemplate(PS.Object);
		SecondTeleportParticleComponent->SetRelativeScale3D(FVector(0.5));
		SecondTeleportParticleComponent->SetIsReplicated(true);
		SecondTeleportParticleComponent->SetAutoActivate(false);
	} 

	ConstructorHelpers::FObjectFinder<USoundWave> SW(TEXT("SoundWave'/Game/TWSFX_FS_Ultimate1/Dirt1/WAV/WAV_TomWinandySFX_-_FS_Dirt_run_12.WAV_TomWinandySFX_-_FS_Dirt_run_12'"));
	if (SW.Succeeded())
	{
		FootAudio->SetSound(SW.Object);
		FootAudio->SetupAttachment(GetMesh(),TEXT("root"));
	}

	ConstructorHelpers::FObjectFinder<USoundWave> SSW(TEXT("SoundWave'/Game/UltimateSFXBundle/wavs/MagicSpells/Shot/magic_spell_shot_15.magic_spell_shot_15'"));
	if (SSW.Succeeded())
	{
		MagicAudio= CreateDefaultSubobject<UAudioComponent>("MagicAudio");
		MagicAudio->SetSound(SSW.Object);
		MagicAudio->SetupAttachment(GetMesh());
		MagicAudio->SetAutoActivate(false);
	}
	
	ConstructorHelpers::FObjectFinder<USoundWave> TSW(TEXT("SoundWave'/Game/UltimateSFXBundle/wavs/MagicSpells/Shot/magic_spell_shot_15.magic_spell_shot_15'"));
	if (TSW.Succeeded())
	{
		TeleportAudio = CreateDefaultSubobject<UAudioComponent>("TeleportAudio");
		TeleportAudio->SetSound(TSW.Object);
		TeleportAudio->SetupAttachment(GetMesh());
		TeleportAudio->SetAutoActivate(false);
	}
}

void ABodyChanger::BeginPlay()
{
	Super::BeginPlay();

	GetComponent<UTwoFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction((GetMesh()->GetAnimInstance()), FName("FootIk"));

	ProjectilePool->InitializePool<AWaterBall>(this);

}

void ABodyChanger::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Acceleration"), EInputEvent::IE_Pressed, this, &ABodyChanger::Teleportation);
	PlayerInputComponent->BindAction(TEXT("SpecialSkill"), EInputEvent::IE_Pressed, this, &ABodyChanger::BodyChange);
}

void ABodyChanger::GetWaterBall_Implementation(FVector Location, FRotator Rotation)
{
	ProjectilePool->GetProjectile<AWaterBall>(Location, Rotation);
}

void ABodyChanger::Teleportation_Implementation()
{

	auto NormalVector = GetComponent<UMoveComponent>(Component::Move)->GetNormalVector();

	if (GetCharacterMovement()->IsFalling() || NormalVector == FVector(0.)) return;
	
	if (!GetComponent<USkillComponent>(Component::Skill)->UseSkill(0)) return;

	FCollisionQueryParams Params(NAME_None, false, this);

	Params.bIgnoreTouches = true;

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() - GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5, ECollisionChannel::ECC_Visibility, Params))
	{
		TeleportParticleComponent->SetWorldLocationAndRotation(GetMesh()->GetComponentLocation() + FVector(0, 0, 1), FRotationMatrix::MakeFromZX(HitResult.ImpactNormal, FVector::ForwardVector).Rotator());
	}

	//TeleportParticleComponent->SetWorldLocationAndRotation(GetMesh()->GetComponentLocation(), LeftFootRotatorValue);

	FVector StartSweep = GetActorLocation() + GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	TArray<FHitResult> HitArray;

	//DrawDebugLine(GetWorld(), GetMesh()->GetComponentLocation() , GetMesh()->GetComponentLocation() + NormalVector * 1000.f, FColor::Green, false, 5.f);
	if (GetWorld()->LineTraceMultiByChannel(HitArray, GetActorLocation(), GetActorLocation() + NormalVector * 1000.f, ECollisionChannel::ECC_Visibility, Params))
	{
		float UpDistance = 1000.f;
		for (auto& Hit : HitArray)
		{
			if (!Cast<ACharacter>(Hit.GetActor()) && !Cast<ALandscapeStreamingProxy>(Hit.GetActor()))
			{
				Params.AddIgnoredActor(Hit.GetActor());
				UpDistance = Hit.Distance;

				break;

			}
		}

		GetWorld()->LineTraceMultiByChannel(HitArray, GetMesh()->GetComponentLocation() - FVector(0., 0., GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), GetMesh()->GetComponentLocation() - FVector(0., 0., GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) + NormalVector * 1000.f, ECollisionChannel::ECC_Visibility, Params);

		if (HitArray.Num() > 1)
		{
			float DownDistance = 1000.f;
			for (auto& Hit : HitArray)
			{
				if (!Cast<ACharacter>(Hit.GetActor()))
				{

					DownDistance = Hit.Distance;

					break;

				}
			}
			StartSweep += (UpDistance <= DownDistance ? UpDistance : DownDistance) * NormalVector;

			if (GetWorld()->SweepMultiByChannel(HitArray, StartSweep, StartSweep - GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5, FQuat::Identity, ECollisionChannel::ECC_Visibility, GetCapsuleComponent()->GetCollisionShape(), Params))
			{
				for (auto& Hit : HitArray)
				{
					if (!Cast<ACharacter>(Hit.GetActor()))
					{

						SetActorLocation(Hit.Location);

						SecondTeleportParticleComponent->SetWorldLocationAndRotation(GetMesh()->GetComponentLocation() + FVector(0, 0, 1), FRotationMatrix::MakeFromZX(Hit.ImpactNormal, FVector::ForwardVector).Rotator());
						break;

					}
				}
			}
		}
		else
		{
			StartSweep += UpDistance * NormalVector;

			if (GetWorld()->SweepMultiByChannel(HitArray, StartSweep, StartSweep - GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5, FQuat::Identity, ECollisionChannel::ECC_Visibility, GetCapsuleComponent()->GetCollisionShape(), Params))
			{
				for (auto& Hit : HitArray)
				{
					if (!Cast<ACharacter>(Hit.GetActor()))
					{

						SetActorLocation(Hit.Location);

						SecondTeleportParticleComponent->SetWorldLocationAndRotation(GetMesh()->GetComponentLocation() + FVector(0, 0, 1), FRotationMatrix::MakeFromZX(Hit.ImpactNormal, FVector::ForwardVector).Rotator());
						break;

					}
				}
			}
		}
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitArray, GetMesh()->GetComponentLocation() - FVector(0., 0., GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), GetMesh()->GetComponentLocation() - FVector(0., 0., GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) + NormalVector * 1000.f, ECollisionChannel::ECC_Visibility, Params);

		if (HitArray.Num() > 1)
		{
			float DownDistance = 1000.f;
			for (auto& Hit : HitArray)
			{

				DownDistance = Hit.Distance;

				break;

			}

			StartSweep += DownDistance * NormalVector;

			if (GetWorld()->SweepMultiByChannel(HitArray, StartSweep, StartSweep - GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5, FQuat::Identity, ECollisionChannel::ECC_Visibility, GetCapsuleComponent()->GetCollisionShape(), Params))
			{
				for (auto& Hit : HitArray)
				{
					if (!Cast<ACharacter>(Hit.GetActor()))
					{

						SetActorLocation(Hit.Location);

						SecondTeleportParticleComponent->SetWorldLocationAndRotation(GetMesh()->GetComponentLocation() + FVector(0, 0, 1), FRotationMatrix::MakeFromZX(Hit.ImpactNormal, FVector::ForwardVector).Rotator());
						break;

					}
				}
			}

		}
		else
		{
			StartSweep += 1000.f * NormalVector;

			if (GetWorld()->SweepMultiByChannel(HitArray, StartSweep, StartSweep, FQuat::Identity, ECollisionChannel::ECC_Visibility, GetCapsuleComponent()->GetCollisionShape(), Params))
			{
				for (auto& Hit : HitArray)
				{
					if (!Cast<ACharacter>(Hit.GetActor()) && !Cast<ALandscapeStreamingProxy>(Hit.GetActor()))
					{
						Params.AddIgnoredActor(Hit.GetActor());

					}
				}
			}

			if (GetWorld()->SweepMultiByChannel(HitArray, StartSweep, StartSweep - GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5, FQuat::Identity, ECollisionChannel::ECC_Visibility, GetCapsuleComponent()->GetCollisionShape(), Params))
			{
				for (auto& Hit : HitArray)
				{
					if (!Cast<ACharacter>(Hit.GetActor()))
					{

						SetActorLocation(Hit.Location);

						SecondTeleportParticleComponent->SetWorldLocationAndRotation(GetMesh()->GetComponentLocation()+FVector(0,0,1), FRotationMatrix::MakeFromZX(Hit.ImpactNormal, FVector::ForwardVector).Rotator());
						break;

					}
				}
			}
		}

	}

	Active();
	
	PlayTeleportSound();
}

void ABodyChanger::PlayMagicSound()
{
	if (!MagicAudio->IsActive())
	{
		MagicAudio->Activate();
	}
	else
	{
		MagicAudio->Play();
	}
}

void ABodyChanger::PlayTeleportSound_Implementation()
{
	if (!TeleportAudio->IsActive())
	{
		TeleportAudio->Activate();
	}
	else
	{
		TeleportAudio->Play();
	}
}

void ABodyChanger::BodyChange_Implementation()
{
	if (GetComponent<UHealthComponent>(Component::Health)->GetMaxHealth() != GetComponent<UHealthComponent>(Component::Health)->GetCurrentHealth()) return;

	ULookComponent* Look = GetComponent<ULookComponent>(Component::Look);
	
	FCollisionQueryParams Params(NAME_None, false, this);
	Params.bIgnoreTouches = true;
	Params.AddIgnoredActor(this);
	FHitResult HitResult;

	if (auto Result = GetWorld()->LineTraceSingleByChannel(HitResult, Look->GetCamera()->GetComponentLocation(), Look->GetCamera()->GetComponentLocation() + GetBaseAimRotation().Vector() * 1000.f, ECollisionChannel::ECC_Visibility, Params))
	{
		if (auto Character = Cast<ABaseCharacter>(HitResult.GetActor()))
		{

			if (!GetComponent<USkillComponent>(Component::Skill)->UseSkill(1)) return;

			auto PlayerController = GetController();
			if (Character->IsPlayerControlled()) Character->GetController()->Possess(this);
			else
			{
				PlayerController->UnPossess();
				Destroy();
			}
			GetCharacterMovement()->StopMovementImmediately();
			PlayerController->Possess(Character);

		}
	}
}

void ABodyChanger::Active_Implementation()
{
	TeleportParticleComponent->Activate();
	SecondTeleportParticleComponent->Activate();
}

void ABodyChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

