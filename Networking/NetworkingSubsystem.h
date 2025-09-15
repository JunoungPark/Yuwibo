// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FTLSConnection.h"
#include "FPacketHandler.h"
#include "NetworkingSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UNetworkingSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void OnPostWorldInit();

	bool Connect(const FString& Host, int32 Port) { return Connection.Connect(Host, Port); }

	bool Signin(std::string UserId, std::string Password);
	bool Signup(std::string UserId, std::string Password);
	bool Register(std::string UserName);

	void CreateRoom(const FText& Text);

	UFUNCTION()
	void FindRoom(const FText& Text);

	bool JoinRoom(uint32 RoomID);
	bool LeaveRoom();

	bool StartGame(std::string URL);

	bool TLSHandshake() { return Connection.TLSHandshake(); }

	// Graceful close
	void Close() { Connection.Close(); }

	TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UMyNetworkSubsystem, STATGROUP_Tickables); }

	FPacketHandler& GetPacketHandler() { return PacketHandler; }
protected:
	void Tick(float DeltaTime)override;
private:

	bool Send(const google::protobuf::Message& Message);

	bool ProcessPacket(Buffer* buffer);

	void HandlePacket(const uint8* data, size_t len);
private:

	TQueue<TUniquePtr<Buffer>> RecvBufQueue;
	TUniquePtr<Buffer> RemainingData;
	TLS Tls;

	FTLSConnection Connection = FTLSConnection(RecvBufQueue, Tls);
	FPacketHandler PacketHandler;

	FThreadSafeBool IsDataRemaining = false;

public:
	// 성공 여부(bool), 에러 코드(int32) 전달하는 델리게이트
	DECLARE_DELEGATE_TwoParams(FOnSigninResponse, bool, UnrealEngineMessage::ErrorCode);
	FOnSigninResponse OnSigninResponse;

	DECLARE_DELEGATE_TwoParams(FOnSignupResponse, bool, UnrealEngineMessage::ErrorCode);
	FOnSignupResponse OnSignupResponse;

	DECLARE_DELEGATE(FOnRegisterResponse);
	FOnRegisterResponse OnRegisterResponse;
	// 방 찾기 같은 경우는 RepeatedPtrField 참조를 넘김
	DECLARE_DELEGATE_OneParam(FOnFindRoomResponse, const google::protobuf::RepeatedPtrField<UnrealEngineMessage::RoomInfo>&);
	FOnFindRoomResponse OnFindRoomResponse;

	DECLARE_DELEGATE_FiveParams(FOnJoinRoomResponse, bool, UnrealEngineMessage::ErrorCode, const UnrealEngineMessage::RoomInfo&, const google::protobuf::RepeatedPtrField<UnrealEngineMessage::UserInfo>&, const std::string);
	FOnJoinRoomResponse OnJoinRoomResponse;

	DECLARE_DELEGATE_ThreeParams(FOnUpdateRoomInfo, const UnrealEngineMessage::RoomInfo&, const google::protobuf::RepeatedPtrField<UnrealEngineMessage::UserInfo>&, const std::string);
	FOnUpdateRoomInfo OnUpdateRoomInfo;

	DECLARE_DELEGATE_TwoParams(FOnBroadcastResponse, const UnrealEngineMessage::UserInfo&, std::string);
	FOnBroadcastResponse OnBroadcastResponse;

	DECLARE_DELEGATE_OneParam(FOnGameResponse, std::string);
	FOnGameResponse OnGameResponse;
};
