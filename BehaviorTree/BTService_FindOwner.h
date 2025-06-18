// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindOwner.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UBTService_FindOwner : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_FindOwner();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
