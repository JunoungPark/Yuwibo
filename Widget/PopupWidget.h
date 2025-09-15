// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Yuwibo/Networking/UnrealEngineMessage.pb.h"
#include "PopupWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UPopupWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NotificationMessage;
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ExitText;
private:
	UFUNCTION()
	void OnExitButtonHovered();

	UFUNCTION()
	void OnExitButtonUnHovered();
	UFUNCTION()
	void Exit();
protected:
	void NativeOnInitialized();
public:
	void OnSigninResponse(bool success, UnrealEngineMessage::ErrorCode errcode);
	void OnSignupResponse(bool success, UnrealEngineMessage::ErrorCode errcode);
	void OnJoinRoomResponse(UnrealEngineMessage::ErrorCode errcode);

};
