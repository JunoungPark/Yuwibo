// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	// ...
}

void UMoveComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMoveComponent, ForwardValue);
	DOREPLIFETIME(UMoveComponent, RightValue);
	DOREPLIFETIME(UMoveComponent, CanMove);
	DOREPLIFETIME(UMoveComponent, CanAttack);
}

void UMoveComponent::MoveUpDown(float value)
{
	if (CanMove)
	{
		auto Vector = Character->GetActorForwardVector();

		Vector.Z = 0;

		Vector.Normalize();

		Character->AddMovementInput(Vector, value);

	}
	ForwardValue = value; 
	SetForwardValue(ForwardValue);
}

void UMoveComponent::MoveLeftRight(float value)
{
	if (CanMove)
	{
		Character->AddMovementInput(Character->GetActorRightVector(), value);

	}
	RightValue = value;
	SetRightValue(RightValue);
}

void UMoveComponent::SetForwardValue_Implementation(float value)
{
	ForwardValue = value;
}

void UMoveComponent::SetRightValue_Implementation(float value)
{
	RightValue = value;
}



void UMoveComponent::SetCanMove_Implementation(bool IsTrue)
{
	CanMove = IsTrue;
}

void UMoveComponent::SetCanAttack_Implementation(bool IsTrue)
{
	CanAttack = IsTrue;
}