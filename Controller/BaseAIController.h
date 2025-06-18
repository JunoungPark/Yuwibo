// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
public:
		ABaseAIController();
protected:
	UPROPERTY()
		class UBehaviorTree* BehaviorTree;
	UPROPERTY()
		class UBlackboardData* BlackboardData;
protected:
	DECLARE_DELEGATE_OneParam(SightSignature, AActor*)
	SightSignature OnSight;
	DECLARE_DELEGATE_TwoParams(NoiseSignature, bool, FVector)
	NoiseSignature OnNoise;
private:

	UFUNCTION()
	void Reaction(AActor* actor, struct FAIStimulus Stimulus);

protected:

	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
