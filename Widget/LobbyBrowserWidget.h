// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online/Lobbies.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "LobbyBrowserWidget.generated.h"

/**
 * 
 */


UCLASS()
class YUWIBO_API ULobbyBrowserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* LobbyNameText;
	UPROPERTY(meta = (BindWidget))
	class UButton* Lobby_Name;
	UPROPERTY(meta = (BindWidget))
	class UButton* Players;
	UPROPERTY(meta = (BindWidget))
	class UListView* Lobby_List;

	UPROPERTY(meta = (BindWidget))
	class UBackgroundBlur* HostGameBlur;
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* HostGame;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* HostName;
	UPROPERTY(meta = (BindWidget))
	class UButton* HostGameButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY()
	class ULobbyWidget* LobbyWidget;
private:
	FORCEINLINE UE::Online::FAccountId GetAccountID() { auto GameInstance = Cast<UYuwiboGameInstance>(GetGameInstance()); return GameInstance ? GameInstance->GetAccountID() : UE::Online::FAccountId(); }

	UFUNCTION()
	void OnItemDoubleClicked(UObject* MyListView);

	UFUNCTION()
	FORCEINLINE void FindLobby(const FText& Text) { UpdateEntry(); }

	void JoinLobby(UE::Online::FLobbyId LobbyID);

	UFUNCTION()
	void CreateLobby();

	UFUNCTION()
	void Cancel();

	UFUNCTION()
	void GameExit();
public:
	void NativeOnInitialized()override;

	void SetLobbyEntry(ULobbyEntryWidget& Entry, UE::Online::FLobby Lobby);

	void CreateLobbyWidget();

	void UpdateEntry();

	void UpdateLobby();
};

UCLASS()
class YUWIBO_API ULobbyEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	friend void ULobbyBrowserWidget::SetLobbyEntry(ULobbyEntryWidget& Entry, UE::Online::FLobby Lobby);
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* LobbyButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Lobby_Name;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayersNum;

	FText LobbyName;
	uint8 PlayersCurrentNum;
	uint8 PlayersMaxNum;

	UE::Online::FLobbyId LobbyID;
private:

	void SetData(ULobbyEntryWidget* Widget);
public:

	FORCEINLINE UE::Online::FLobbyId GetLobbyID() { return LobbyID; }


	void NativeOnListItemObjectSet(UObject* ListItemObject);

};

UCLASS()
class YUWIBO_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

	friend void ULobbyBrowserWidget::UpdateLobby();

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
	class UTextBlock* StartGame;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ExitLobby;
private:
	FORCEINLINE UE::Online::FAccountId GetAccountID() { auto GameInstance = Cast<UYuwiboGameInstance>(GetGameInstance()); return GameInstance ? GameInstance->GetAccountID() : UE::Online::FAccountId(); }

	UFUNCTION()
	void GameStart();
	UFUNCTION()
	void LeaveLobby();

	UFUNCTION()
	void OnStartButtonHovered();

	UFUNCTION()
	void OnStartButtonUnHovered();

	UFUNCTION()
	void OnExitButtonHovered();

	UFUNCTION()
	void OnExitButtonUnHovered();
protected:
	void NativeOnInitialized()override;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
};