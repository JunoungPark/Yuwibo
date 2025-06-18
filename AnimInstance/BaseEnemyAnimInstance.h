// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "BaseEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UBaseEnemyAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

	friend void UEnemyAnimComponent::OnRep_UpdateAnimationValues();

protected:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool IsLook = false; 										
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool IsAI;										
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		AActor* Target = nullptr;									
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		FVector LookVector;											
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		FRotator LookRotator;
public:
	//UFUNCTION()
	//FORCEINLINE void SetTarget(AActor* NewTarget) { Target = NewTarget; };
	//
	void AnimNotify_Search()override;
	//
	//virtual void NativeUpdateAnimation(float DeltaSeconds)override;
	//
	//UFUNCTION()
	//void SetLook(bool IsTrue, FVector Vector);
//
	FORCEINLINE bool GetLook() { return IsLook; }
//
	//void AILook(float DeltaSeconds);
//
	//void AIMove();
};

UCLASS()
class YUWIBO_API UTwoFootEnemyAnimInstance : public UBaseEnemyAnimInstance
{
	GENERATED_BODY()
protected:
		UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
		FVector HipOffset;
		UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
		FRotator LeftFootRotatorValue;
		UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
		FRotator RightFootRotatorValue;
		UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
		FVector LeftFootEffectorLocation;
		UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
		FVector RightFootEffectorLocation;

private:

	UFUNCTION()
	void FootIk(struct FTwoFootIkStruct FootStruct);
};

UCLASS()
class YUWIBO_API UFourFootEnemyAnimInstance : public UBaseEnemyAnimInstance
{
	GENERATED_BODY()
protected:


	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector HipOffset;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator LeftFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator RightFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator BackLeftFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FRotator BackRightFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector LeftFootEffectorLocation;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector RightFootEffectorLocation;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector BackLeftFootEffectorLocation;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly)
	FVector BackRightFootEffectorLocation;
private:

	UFUNCTION()
	void FootIk(struct FFourFootIkStruct FootStruct);
};

//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	 //bool LookCharacter;
//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	 //bool IsFirstTurn;
//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	 //float LookAlpha;
//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	 //FRotator BodyRotator;
//UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
	 //FRotator SpineRotator;


//FORCEINLINE void SetIsTurn(bool IsTurn) { IsFirstTurn = IsTurn; }

//UFUNCTION(BlueprintPure)
//FORCEINLINE bool GetIsTurn() const { return IsFirstTurn; };
//UFUNCTION(BlueprintPure)
//FORCEINLINE FRotator GetSpineRotator() const { return SpineRotator; };
									  //UFUNCTION(BlueprintPure)
//FORCEINLINE FRotator GetLookRotator() const { return LookRotator; };