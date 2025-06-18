// Fill out your copyright notice in the Description page of Project Settings.


#include "LookComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/YuwiboGameModeBase.h"

// Sets default values for this component's properties

ULookComponent::ULookComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	//SetIsReplicated(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->bUsePawnControlRotation = true;
	Camera->SetupAttachment(SpringArm);
	Camera->SetIsReplicated(true);
	Camera->bOverrideAspectRatioAxisConstraint = 1;

	SpringArm->TargetArmLength = 300.f;
	SpringArm->SocketOffset = FVector(0., -125., 75.);
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
}

void ULookComponent::Bind(ABaseCharacter* NewCharacter)
{
	Character = NewCharacter;

	SpringArm->SetupAttachment(Character->GetCapsuleComponent());
}

void ULookComponent::LookUpDown(float value)
{
	Character->AddControllerPitchInput(value);
}

void ULookComponent::LookLeftRight(float value)
{
	if (!AttackLookMove && Character->GetIsAttack())
		return;

	Character->AddControllerYawInput(value);

}

