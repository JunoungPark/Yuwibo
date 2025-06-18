// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "Crustaspikan.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ACrustaspikan : public AMoveCharacter
{
	GENERATED_BODY()
		ACrustaspikan();

public:

	void BeginPlay() override;
};
