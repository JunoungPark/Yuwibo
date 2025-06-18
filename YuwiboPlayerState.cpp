// Fill out your copyright notice in the Description page of Project Settings.


#include "YuwiboPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "YuwiboGameModeBase.h"

void AYuwiboPlayerState::AddScore_Implementation(uint8 Value)
{
	auto NewValue = GetScore() + Value;
	
	if (NewValue >= 3) static_cast<AYuwiboGameModeBase*>(UGameplayStatics::GetGameMode(this))->EndMatch();
	else SetScore(NewValue);
}
