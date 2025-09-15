// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Yuwibo/Character/BaseCharacter.h"
//#include "Online/Lobbies.h"
#include "YuwiboGameModeBase.generated.h"

UCLASS()
class YUWIBO_API AYuwiboTestModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AYuwiboTestModeBase();

};

UCLASS()
class YUWIBO_API AYuwiboSigninModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AYuwiboSigninModeBase();

};

UCLASS()
class YUWIBO_API AYuwiboLobbyModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AYuwiboLobbyModeBase();
	
};

UCLASS()
class YUWIBO_API AYuwiboGameModeBase : public AGameMode
{
	GENERATED_BODY()

	friend class AYuwiboPlayerState;

	AYuwiboGameModeBase();
private:
	TArray<TSubclassOf<ABaseCharacter>> AvailableCharacters;

	uint8 PlayerNum = 0;

	uint8 RemainingPlayer = 0;

	FTimerHandle WorldTimerHandle;

	FTimerHandle StartMatchHandle;

	class AATimerManager* TimerManagerInstance;

	bool Alarm = false;

	UPROPERTY()
	TArray<class AStoneStatue*> Statues;

	UPROPERTY()
	TArray<class AStoneStatue*> VisibleStatues;
private:
	void VisibleStoneStatue();

	void PostLogin(APlayerController* NewPlayer)override;

	void Logout(AController* Exiting)override;

protected:
	void CheckPlayerNum();

	void HandleMatchHasStarted() override;

	void HandleMatchHasEnded()override;
public:

	UFUNCTION(Server, Reliable)
	void SendLobbyNum(uint8 LobbyNum);

	void RemoveVisibleStatue(class AStoneStatue* Statue) { VisibleStatues.Remove(Statue); };

	void VisibleStatueWIdget(ABaseCharacter* Character);

	UFUNCTION(Server, Reliable)
	void SetPlayerLocation(class ABasePlayerController* Controller, uint8 Index);

	void BeginPlay() override;

	void PlayerIsEnd(ABasePlayerController* Controller);

	AATimerManager* GetTimer() { return TimerManagerInstance; }
};
