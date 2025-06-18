// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ResetFailMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UBTService_ResetFailMoveTo : public UBTService
{
	GENERATED_BODY()
	
	

	UBTService_ResetFailMoveTo();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
