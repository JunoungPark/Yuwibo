// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LookComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YUWIBO_API ULookComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	ULookComponent();

private:
	UPROPERTY(VisibleAnywhere)
	   class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	   class UCameraComponent* Camera;
	UPROPERTY(Category = Move, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	   class ABaseCharacter* Character;

	   bool AttackLookMove = true;
public:	

	FORCEINLINE class USpringArmComponent* GetSpringArm() { return SpringArm; };

	FORCEINLINE class UCameraComponent* GetCamera() { return Camera; };

	FORCEINLINE void SetAttackLookMove() { AttackLookMove = false; }

	void Bind(class ABaseCharacter* NewCharacter);

	void LookUpDown(float value);

	void LookLeftRight(float value);
		
};
