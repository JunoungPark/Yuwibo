// Fill out your copyright notice in the Description page of Project Settings.


#include "BowAnimInstance.h"
#include "Yuwibo/Character/Trainer.h"

void UBowAnimInstance::NativeInitializeAnimation()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Character = Cast<ATrainer>(Pawn);
	}
}

void UBowAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Character)
	{
		StringLocation = Character->GetMesh()->GetSocketLocation("middle_03_r");

		if (Character->GetCatchString())
		{
			StringAlpha = 1.f;
		}
		else
		{
			StringAlpha = FMath::FInterpTo(StringAlpha, 0, DeltaSeconds, 20);
		}
	}
}
