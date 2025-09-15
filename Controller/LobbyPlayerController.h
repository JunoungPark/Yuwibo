// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	ALobbyPlayerController();

private:
	UPROPERTY()
	class ULobbyBrowserWidget* LobbyBrowserWidget = nullptr;
	
	UPROPERTY()
	class ULobbyWidget* LobbyWidget = nullptr;

	UPROPERTY()
	class UPopupWidget* PopupWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbyBrowserWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PopupWidgetClass;

protected:
	void BeginPlay()override;
public:

	UPopupWidget* GetPopup();

	ULobbyBrowserWidget* GetLobbyBrowserWidget() { return LobbyBrowserWidget; }
	ULobbyWidget* GetLobbyWidget();



};
