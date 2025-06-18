// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FailMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UBTTask_FailMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FailMoveTo();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
