// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Yuwibo/Networking/UnrealEngineMessage.pb.h"
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

	UFUNCTION()
	void OnItemDoubleClicked(UObject* MyListView);

	UFUNCTION()
	void CreateRoom();

#ifdef EOS
	void JoinLobby(UE::Online::FLobbyId LobbyID);

	void CreateLobby();
#endif

	UFUNCTION()
	void Cancel();

	UFUNCTION()
	void GameExit();
public:
	void NativeOnInitialized()override;

	void SetLobbyEntry(ULobbyEntryWidget& Entry, UnrealEngineMessage::RoomInfo Room);

	void UpdateEntry(const google::protobuf::RepeatedPtrField<UnrealEngineMessage::RoomInfo>& Rooms);

#ifdef EOS
	FORCEINLINE void FindLobby(const FText& Text) { UpdateEntry(); }

	void SetLobbyEntry(ULobbyEntryWidget& Entry, UE::Online::FLobby Lobby);
	void UpdateEntry();
	void UpdateLobby();
#endif

	std::string GetSearchLobbyName();
};

UCLASS()
class YUWIBO_API ULobbyEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	friend void ULobbyBrowserWidget::SetLobbyEntry(ULobbyEntryWidget& Entry, UnrealEngineMessage::RoomInfo Room);
	//friend void ULobbyBrowserWidget::SetLobbyEntry(ULobbyEntryWidget& Entry, UE::Online::FLobby Lobby);
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* LobbyButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Lobby_Name;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayersNum;
	
	uint32 LobbyID;
	FText LobbyName;
	uint8 PlayersCurrentNum;
	uint8 PlayersMaxNum;

	//UE::Online::FLobbyId LobbyID;
private:

	void SetData(ULobbyEntryWidget* Widget);
public:

	FORCEINLINE uint32 GetLobbyID() { return LobbyID; }

	//FORCEINLINE UE::Online::FLobbyId GetLobbyID() { return LobbyID; }

	void NativeOnListItemObjectSet(UObject* ListItemObject);

};

