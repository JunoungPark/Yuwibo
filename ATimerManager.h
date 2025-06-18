// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATimerManager.generated.h"

UCLASS()
class YUWIBO_API AATimerManager : public AActor
{
    GENERATED_BODY()

public:
    AATimerManager();


    DECLARE_EVENT(AATimerManager, FTickEvent)
    FTickEvent OnTickEvent;

private:
    FTimerHandle TickTimerHandle;

protected:
    virtual void BeginPlay() override;

private:
    void CallTickDelegates();
};