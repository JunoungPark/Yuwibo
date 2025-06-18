// Fill out your copyright notice in the Description page of Project Settings.


#include "Moose.h"
#include "Yuwibo/Component/AiComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Component/FootIkComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kismet/GameplayStatics.h"
#include "Yuwibo/Animinstance/MooseAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"

AMoose::AMoose()
{
	SetTeamId(FGenericTeamId(101));

	CharacterName = "Moose";

	SetComponent<UEnemyComponent>(Component::Enemy);

	SetComponent<UFourFootIkComponent>(Component::FootIk)->BindSocket("leg_f_L_5", "leg_f_R_5", "leg_b_L_5", "leg_b_R_5");

	//GetComponent<ULookComponent>(Component::Look)->GetSpringArm()->TargetArmLength = 600.;
	//GetComponent<ULookComponent>(Component::Look)->GetSpringArm()->SocketOffset = FVector(0., -125., 0.);

	GetComponent<UMoveComponent>(Component::Move)->SetAttackCanMove();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMoose::HitBody);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	ConstructorHelpers::FClassFinder<UMooseAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/Moose/MooseAB.MooseAB_C'"));
	if (AB.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB.Class);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Masked_Moose/Mesh/SK_Masked_Moose.SK_Masked_Moose'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	}
	GetCharacterMovement()->JumpZVelocity = 600.f;
}

void AMoose::MoveUpDown(float value)
{
	if (auto Move = GetComponent<UMoveComponent>(Component::Move))
	{
		if(Move->GetCanMove())
			AddMovementInput(FRotator(0., GetActorRotation().Yaw + GetComponent<UMooseAnimComponent>(Component::Anim)->GetRootYaw(), 0.).Vector(), value);
		
		Move->SetForwardValue(value); 
	}
}

void AMoose::MoveLeftRight(float value)
{
	if (auto Move = GetComponent<UMoveComponent>(Component::Move))
	{
		Move->SetRightValue(value);
		if (Move->GetCanMove() && !GetCharacterMovement()->IsFalling())
		{
			if (Move->GetForwardValue() < 0)
			{
				value = -value;
			}
			GetComponent<UMooseAnimComponent>(Component::Anim)->AddRootYaw(value * 3);

		}
	}
}

//	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMoose::HitBody);
//	//UE_LOG(LogTemp, Log, TEXT("%f"), RightFootOffset);


void AMoose::HitBody(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character )
	{

		if (Character != Owner)
		{
			UGameplayStatics::ApplyDamage(Character, UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) / 50, GetInstigator()->Controller, this, UDamageType::StaticClass());
		}
		auto LaunchVelocity = FRotator(0., GetActorRotation().Yaw + GetComponent<UMooseAnimComponent>(Component::Anim)->GetRootYaw(), 0.).Vector() * UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) * 1.5; // �з����� ����� �� ����
		

		if (50 > LaunchVelocity.X + LaunchVelocity.Y)
		{
			LaunchVelocity *= 2;
		}

		LaunchVelocity.Z = 200.0f; // ��¦ ���� ƨ�ܼ� �ڿ�������

		Character->LaunchCharacter(LaunchVelocity, true, true);

		//Character->GetCharacterMovement()->AddImpulse(FRotator(0., GetActorRotation().Yaw + Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance())->GetRootYaw(), 0.).Vector() * UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) * 20, true);
		//Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance())->PlayAttackAnimation();
		
	}
}

void AMoose::BeginPlay()
{
	Super::BeginPlay();

	GetComponent<UFourFootIkComponent>(Component::FootIk)->OnFootIk.BindUFunction(GetMesh()->GetAnimInstance(), FName("MooseFootIk"));
}

void AMoose::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveUpDown"), this, &AMoose::MoveUpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftRight"), this, &AMoose::MoveLeftRight);
}

void AMoose::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetComponent<UMoveComponent>(Component::Move)->GetForwardValue() > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->GetMaxSpeed(), 1900.f, DeltaTime, 10);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->GetMaxSpeed(), 200.f, DeltaTime, 10);
	}
}

void AMoose::Jump()
{
	if (GetComponent<UMoveComponent>(Component::Move)->GetForwardValue() > 0)
		Super::Jump();
}
