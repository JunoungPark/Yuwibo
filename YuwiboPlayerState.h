// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "YuwiboPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API AYuwiboPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void AddScore(uint8 Value = 1);
};
