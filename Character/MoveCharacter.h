// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MoveCharacter.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API AMoveCharacter : public ABaseCharacter
{
	GENERATED_BODY()
protected:

	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* FootAudio;
private:
	UFUNCTION()
	virtual void IsClient(bool IsTrue);
	UFUNCTION(Server, Reliable)
	void LocallyControll(bool IsTrue);

	UFUNCTION(NetMulticast, Reliable)
	void IsServer(bool IsTrue);
public:
	AMoveCharacter();

public:

	void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayFootSound();
};
