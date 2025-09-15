// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SigninPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ASigninPlayerController : public APlayerController
{
	GENERATED_BODY()

	ASigninPlayerController();
private:

	UPROPERTY()
	UUserWidget* CurrentWidget = nullptr;

	UPROPERTY()
	class UPopupWidget* PopupWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PopupWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SigninWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> UserNameWidgetClass;

protected:
	void BeginPlay()override;

public:

	void SignupModeChange();
	void ShowUserNameWidget();
	UPopupWidget* GetPopup();

};
