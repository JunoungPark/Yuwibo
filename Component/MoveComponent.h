// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "MoveComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YUWIBO_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Move, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	ACharacter* Character;

	UPROPERTY(Replicated)
	float ForwardValue;

	UPROPERTY(Replicated)
	float RightValue;

	UPROPERTY(Replicated)
	bool CanMove = true;

	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	bool CanAttack = true;

	bool CrouchCanMove = true;
	bool AttackCanMove = false;
public:	
	// Sets default values for this component's properties
	UMoveComponent();

	DECLARE_EVENT(UMoveComponent, FAttackEvent)
	FAttackEvent OnAttack;

	DECLARE_EVENT_OneParam(UMoveComponent, FOnOffAttackEvent, bool)
	FOnOffAttackEvent OnOffAttack;

	DECLARE_DELEGATE_OneParam(FCrouchSignature, bool)
	FCrouchSignature OnOffCrouch;
protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:	
	// Called every frame

	void MoveUpDown(float value);
	void MoveLeftRight(float value);

	UFUNCTION(Server, Reliable)
	void SetForwardValue(float value);

	UFUNCTION(Server, Reliable)
	void SetRightValue(float value);

	UFUNCTION(Server, Reliable)
	void SetCanMove(bool IsTrue);

	UFUNCTION(Server, Reliable)
	void SetCanAttack(bool IsTrue);
	
	UFUNCTION()
	FORCEINLINE void OnRep_CanAttack() { if(!CanAttack) SetAttack<false>(); }

	FORCEINLINE void SetCrouchCanMove() { CrouchCanMove = false; }
	FORCEINLINE void SetAttackCanMove() { AttackCanMove = true; }

	FORCEINLINE float GetForwardValue() { return ForwardValue; }
	FORCEINLINE float GetRightValue() { return RightValue; }
	FORCEINLINE bool GetCanMove() { return CanMove; }
	FORCEINLINE bool GetCanAttack() { return CanAttack; }

	template<bool T>
	FORCEINLINE void SetCrouch() 
	{
		if (!CrouchCanMove)
		{
			if (T)
				CanMove = false;
			else
				CanMove = true;
		}
		OnOffCrouch.ExecuteIfBound(T);
	}

	template<bool T>
	FORCEINLINE void SetAttack()
	{

		if (T)
		{
			if (!CanAttack) return;

			if (!AttackCanMove)
				CanMove = false;

			OnAttack.Broadcast();

		}
		else if (!AttackCanMove)
		{
			CanMove = true;
		}

		OnOffAttack.Broadcast(T);
	}

	FORCEINLINE FVector GetNormalVector()
	{
		return  (Character->GetActorForwardVector() * ForwardValue + Character->GetActorRightVector() * RightValue).GetSafeNormal2D();
	}

	FORCEINLINE void Bind(ACharacter* NewCharacter) { Character = NewCharacter; }
};