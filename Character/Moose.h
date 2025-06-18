// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveCharacter.h"
#include "Moose.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API AMoose : public AMoveCharacter
{
	GENERATED_BODY()

		AMoose();
private:
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	 ABaseCharacter* Rider;
private:

	UFUNCTION()
	void HitBody(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	void MoveUpDown(float value);
	void MoveLeftRight(float value);

	void BeginPlay() override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Tick(float DeltaTime)override;

	void Jump()override;

	FORCEINLINE void  SetRider(ABaseCharacter* Trainer) { Rider = Trainer; };

};
