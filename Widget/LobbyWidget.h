// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Yuwibo/Networking/UnrealEngineMessage.pb.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* FirstMemberButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* SecondMemberButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ThirdMemberButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* LeaveLobbyButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FirstMember;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SecondMember;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ThirdMember;
	UPROPERTY(meta = (BindWidget))
	class UBorder* StartGameBorder;
	UPROPERTY(meta = (BindWidget))
	class UBorder* LeaveLobbyBorder;
private:

	UFUNCTION()
	void GameStart();
	UFUNCTION()
	void LeaveLobby();

	UFUNCTION()
	void OnStartButtonHovered();

	UFUNCTION()
	void OnStartButtonUnHovered();

	UFUNCTION()
	void OnLeaveLobbyButtonHovered();

	UFUNCTION()
	void OnLeaveLobbyButtonUnHovered();
protected:
	void NativeOnInitialized()override;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
public:
	void UpdateLobby(const UnrealEngineMessage::RoomInfo& Room, const google::protobuf::RepeatedPtrField<UnrealEngineMessage::UserInfo>& Users, const std::string MyID);
};