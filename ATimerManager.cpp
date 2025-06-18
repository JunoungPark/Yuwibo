// Fill out your copyright notice in the Description page of Project Settings.


#include "ATimerManager.h"

// Sets default values
AATimerManager::AATimerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AATimerManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &AATimerManager::CallTickDelegates, 0.1f, true);
}

void AATimerManager::CallTickDelegates()
{
	OnTickEvent.Broadcast();
}