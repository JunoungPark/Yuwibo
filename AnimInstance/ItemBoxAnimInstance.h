// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ItemBoxAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UItemBoxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class AItemBox* ItemBox;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool IsOpen;
public:

	FORCEINLINE void SetIsOpen(bool IsTrue) { IsOpen = IsTrue; };
};
