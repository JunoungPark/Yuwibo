// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UTeleportationWidget* TeleportationWidget = nullptr;

	UPROPERTY()
	class UPointWidget* PointWidget = nullptr;

	UPROPERTY()
	class UMenuWidget* MenuWidget = nullptr;

	UPROPERTY()
	class UUserWidget* LoadingWidget = nullptr;

	UPROPERTY()
	class UEndMatchWidget* EndMatchWidget = nullptr;
protected:

	void OnPossess(APawn* InPawn)override;
	void OnUnPossess()override;

	void BeginPlay()override;

private:
	UFUNCTION(Client, Reliable)
	void ClientPossess(APawn* InPawn);
	UFUNCTION(Client, Reliable)
	void ClientUnPossess();	
	UFUNCTION(Server, Reliable)
	void SendPlayerNum(uint8 PlayerNum);
	UFUNCTION(Server, Reliable)
	void SendTeleportationLocation(uint8 Index);

	void MatchEnd();
protected:
	void SetupInputComponent() override;
public:
	UFUNCTION(Client, Reliable)
	void GetPlayerNum();
	UFUNCTION(Client, Reliable)
	void AddViewportWidget(); 
	UFUNCTION(Client, Reliable)
	void AddPoint();
	UFUNCTION(Client, Reliable)
	void GetTeleportationLocation();
	UFUNCTION(Client, Reliable)
	void AddViewportEndMatchWidget(uint8 MaxPlayer, uint8 Rank);
	UFUNCTION(Client, Reliable)
	void EndMatch(uint8 MaxPlayer, uint8 Rank = 1);

	void OnOffMenu();
};
