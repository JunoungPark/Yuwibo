// Fill out your copyright notice in the Description page of Project Settings.


#include "FootIkComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values for this component's properties
UFootIkComponent::UFootIkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFootIkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

float UFootIkComponent::FootRotation(FVector SocketLocation, FRotator& FootRotator, float TraceDistance, float DeltaTime)
{
	float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	float ZLocation = Character->GetActorLocation().Z;
	
	FCollisionQueryParams Params(NAME_None, false, Character);
	Params.AddIgnoredComponent(Character->GetMesh());
	FHitResult HitResult;
	
	bool Result = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(SocketLocation.X, SocketLocation.Y, ZLocation), FVector(SocketLocation.X, SocketLocation.Y, (ZLocation - CapsuleHalfHeight) - TraceDistance), ECollisionChannel::ECC_EngineTraceChannel2, Params);
	if (Result)
	{
		while (Cast<UShapeComponent>(HitResult.Component))
		{
			Params.AddIgnoredComponent(HitResult.Component.Get());
			Result = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(SocketLocation.X, SocketLocation.Y, ZLocation), FVector(SocketLocation.X, SocketLocation.Y, (ZLocation - CapsuleHalfHeight) - TraceDistance), ECollisionChannel::ECC_EngineTraceChannel2, Params);
		}
		while (Cast<ACharacter>(HitResult.GetActor()))
		{
			Params.AddIgnoredActor(HitResult.GetActor());
			Result = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(SocketLocation.X, SocketLocation.Y, ZLocation), FVector(SocketLocation.X, SocketLocation.Y, (ZLocation - CapsuleHalfHeight) - TraceDistance), ECollisionChannel::ECC_EngineTraceChannel2, Params);
		}
		float FootOffset;
	
		FVector ImpactLocation = HitResult.ImpactNormal;
		if (HitResult.IsValidBlockingHit() == true)
		{
			float ImpactLegth = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();
			FootOffset = ImpactLegth - TraceDistance;
	
		}
		else
		{
			FootOffset = 0.0f;
		}
	
		float fAtan2_1 = UKismetMathLibrary::DegAtan2(ImpactLocation.Y, ImpactLocation.Z);
		float fAtan2_2 = UKismetMathLibrary::DegAtan2(ImpactLocation.X, ImpactLocation.Z);
		fAtan2_2 *= -1.0f;
	
		FootRotator = UKismetMathLibrary::RInterpTo(FootRotator, FRotator(fAtan2_2, 0.0f, fAtan2_1), DeltaTime, 13.f);
	
		FColor DrawColor;
	
	
		if (Result)
		{
			DrawColor = FColor::Green;
	
		}
		else
		{
			DrawColor = FColor::Red;
	
		}
		//DrawDebugLine(GetWorld(), FVector(SocketLocation.X, SocketLocation.Y, ZLocation), FVector(SocketLocation.X, SocketLocation.Y, (ZLocation - CapsuleHalfHeight) - TraceDistance), DrawColor);
	
		return FootOffset;
	}


	return false;
}

void UFootIkComponent::Bind(ACharacter* NewCharacter)
{
	Character = NewCharacter;
}

void UTwoFootIkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FootIk(DeltaTime);
	
}

void UTwoFootIkComponent::BindSocket(FName FirstSocketName, FName SecondSocketName)
{
	FirstSocket = FirstSocketName;
	SecondSocket = SecondSocketName;
}

void UTwoFootIkComponent::FootIk(float DeltaTime)
{
	if (Character->GetCharacterMovement()->IsFalling())
	{
		return;
	}
	//if (Character->GetCharacterMovement()->IsFalling())
	//{
	//	return;
	//}
	//float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	float LeftOffset = FootRotation(Character->GetMesh()->GetSocketLocation(FirstSocket), LeftFootRotatorValue, 70.f, DeltaTime);
	float RightOffset = FootRotation(Character->GetMesh()->GetSocketLocation(SecondSocket), RightFootRotatorValue, 70.f, DeltaTime);

	float HipsOffset = UKismetMathLibrary::Min(LeftOffset, RightOffset);

	if (HipsOffset < 0.0f == false) HipsOffset = 0.0f;

	//float CapsuleHalf = 0.0f;
	//
	//bool ResetDefault = true;
	//if (ResetDefault == true)
	//{
	//	CapsuleHalf = CapsuleHalfHeight;
	//}
	//else
	//{
	//	float HalfAbsSize = UKismetMathLibrary::Abs(HipsOffset) * 0.5f;
	//	CapsuleHalf = CapsuleHalfHeight - HalfAbsSize;
	//}
	//
	////! Set capsule component height with FInterpTo 
	//float InterpValue = UKismetMathLibrary::FInterpTo(CapsuleHalfHeight, CapsuleHalf, DeltaTime, 13.f);
	//GetCapsuleComponent()->SetCapsuleHalfHeight(InterpValue, true);

	HipOffset = UKismetMathLibrary::FInterpTo(HipOffset, HipsOffset, DeltaTime, 13.f);

	LeftFootOffset = UKismetMathLibrary::FInterpTo(LeftFootOffset, LeftOffset - HipsOffset, DeltaTime, 13.f);
	RightFootOffset = UKismetMathLibrary::FInterpTo(RightFootOffset, RightOffset - HipsOffset, DeltaTime, 13.f);

	FTwoFootIkStruct FootStruct =
	{
		HipOffset,

		LeftFootRotatorValue,
		RightFootRotatorValue,
		LeftFootOffset,
		RightFootOffset
	};
	OnFootIk.ExecuteIfBound(FootStruct);
}

void UFourFootIkComponent::BindSocket(FName FirstSocketName, FName SecondSocketName, FName ThirdSocketName, FName FourthSocketName)
{
	FirstSocket = FirstSocketName;
	SecondSocket = SecondSocketName;
	ThirdSocket = ThirdSocketName;
	FourthSocket = FourthSocketName;
}

void UFourFootIkComponent::FootIk(float DeltaTime)
{
	if (Character->GetCharacterMovement()->IsFalling())
	{
		return;
	}
	float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	float LeftOffset = FootRotation(Character->GetMesh()->GetSocketLocation(FirstSocket), LeftFootRotatorValue, 70.f, DeltaTime);
	float RightOffset = FootRotation(Character->GetMesh()->GetSocketLocation(SecondSocket), RightFootRotatorValue, 70.f, DeltaTime);
	float BackLeftOffset = FootRotation(Character->GetMesh()->GetSocketLocation(ThirdSocket), BackLeftFootRotatorValue, 70.f, DeltaTime);
	float BackRightOffset = FootRotation(Character->GetMesh()->GetSocketLocation(FourthSocket), BackRightFootRotatorValue, 70.f, DeltaTime);


	float HipsOffset = UKismetMathLibrary::Min(LeftOffset, RightOffset);
	float SecoondHipsOffset = UKismetMathLibrary::Min(BackLeftOffset, BackRightOffset);
	float FinalHipsOffset = UKismetMathLibrary::Min(HipsOffset, SecoondHipsOffset);

	if (FinalHipsOffset < 0.0f == false) FinalHipsOffset = 0.0f;

	float CapsuleHalf = 0.0f;

	bool ResetDefault = true;
	if (ResetDefault == true)
	{
		CapsuleHalf = CapsuleHalfHeight;
	}
	else
	{
		float HalfAbsSize = UKismetMathLibrary::Abs(FinalHipsOffset) * 0.5f;
		CapsuleHalf = CapsuleHalfHeight - HalfAbsSize;
	}

	//! Set capsule component height with FInterpTo 
	float InterpValue = UKismetMathLibrary::FInterpTo(CapsuleHalfHeight, CapsuleHalf, DeltaTime, 13.f);
	Character->GetCapsuleComponent()->SetCapsuleHalfHeight(InterpValue, true);

	HipOffset = UKismetMathLibrary::FInterpTo(HipOffset, FinalHipsOffset, DeltaTime, 13.f);

	LeftFootOffset = UKismetMathLibrary::FInterpTo(LeftFootOffset, LeftOffset - FinalHipsOffset, DeltaTime, 13.f);
	RightFootOffset = UKismetMathLibrary::FInterpTo(RightFootOffset, RightOffset - FinalHipsOffset, DeltaTime, 13.f);
	BackLeftFootOffset = UKismetMathLibrary::FInterpTo(BackLeftFootOffset, BackLeftOffset - FinalHipsOffset, DeltaTime, 13.f);
	BackRightFootOffset = UKismetMathLibrary::FInterpTo(BackRightFootOffset, BackRightOffset - FinalHipsOffset, DeltaTime, 13.f);
	
	FFourFootIkStruct FootStruct =
	{
		HipOffset,

		LeftFootRotatorValue,
		RightFootRotatorValue,
		BackLeftFootRotatorValue,
		BackRightFootRotatorValue,

		LeftFootOffset,
		RightFootOffset,
		BackLeftFootOffset,
		BackRightFootOffset
	};
	OnFootIk.ExecuteIfBound(FootStruct);
}

void UFourFootIkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FootIk(DeltaTime);
	
}
