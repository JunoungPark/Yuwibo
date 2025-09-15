// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkingSubsystem.h"
#include "Yuwibo/Widget/SigninPageWidget.h"
#include "Yuwibo/Widget/UserNameWidget.h"
#include "Yuwibo/Widget/PopupWidget.h"
#include "Yuwibo/Widget/LobbyBrowserWidget.h"
#include "Yuwibo/Widget/LobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/Controller/SigninPlayerController.h"
#include "Yuwibo/Controller/LobbyPlayerController.h"
void UNetworkingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    PacketHandler.Init();

    OnSigninResponse.BindLambda([this](bool success, UnrealEngineMessage::ErrorCode errcode)
        {
            if (success)
            {
                if (errcode == UnrealEngineMessage::ErrorCode::USER_NAME_NONE)
                {
                    if (auto PC = Cast<ASigninPlayerController>(UGameplayStatics::GetPlayerController(this, 0))) PC->ShowUserNameWidget();
                }
                else UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LobbyLevel")));
            }
            else if (auto PC = Cast<ASigninPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
                    if (auto Popup = PC->GetPopup()) Popup->OnSigninResponse(success, errcode);
        });

    OnSignupResponse.BindLambda([this](bool success, UnrealEngineMessage::ErrorCode errcode)
        {
            UE_LOG(LogTemp, Log, TEXT("OnSignupResponse"));
            if (auto PC = Cast<ASigninPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
            {
                UE_LOG(LogTemp, Log, TEXT("OnSignupResponse PC"));
                if(success) PC->SignupModeChange();
                if (auto Popup = PC->GetPopup()) Popup->OnSignupResponse(success, errcode);
            }
        });

    OnRegisterResponse.BindLambda([this]()
        {
            UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LobbyLevel")));
        });

    OnFindRoomResponse.BindLambda([this](const google::protobuf::RepeatedPtrField<UnrealEngineMessage::RoomInfo>& Rooms)
        {
            if(auto PC = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
                    if(auto Lobby = PC->GetLobbyBrowserWidget()) Lobby->UpdateEntry(Rooms);
        });

    OnJoinRoomResponse.BindLambda([this](bool success, UnrealEngineMessage::ErrorCode errcode, const UnrealEngineMessage::RoomInfo& Room, const google::protobuf::RepeatedPtrField<UnrealEngineMessage::UserInfo>& Users, const std::string MyID)
        {
            if (auto PC = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
            {
                if (success)
                {
                    if (auto Lobby = PC->GetLobbyWidget()) Lobby->UpdateLobby(Room, Users, MyID);
                }
                else if(auto Popup = PC->GetPopup()) Popup->OnJoinRoomResponse(errcode);
            }
        });

    OnUpdateRoomInfo.BindLambda([this](const UnrealEngineMessage::RoomInfo& Room, const google::protobuf::RepeatedPtrField<UnrealEngineMessage::UserInfo>& Users, const std::string MyID)
        {
            if (auto PC = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
            {
                if (auto Lobby = PC->GetLobbyWidget()) Lobby->UpdateLobby(Room, Users, MyID);
            }
        });

    OnBroadcastResponse.BindLambda([this](const UnrealEngineMessage::UserInfo& User, std::string Message)
        {
        });

    OnGameResponse.BindLambda([this](std::string URL)
        { 
            UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientTravel(FString(URL.c_str()), TRAVEL_Absolute);
        });
}

bool UNetworkingSubsystem::Signin(std::string UserId, std::string Password)
{
    if (!Connection.GetIsConnect())
    {
        UE_LOG(LogTemp, Log, TEXT("Is Not Connect"));
        return false;
    }

    UnrealEngineMessage::SigninRequest Signin;

	Signin.set_user_id(UserId);
	Signin.set_password(Password);

	return Send(Signin);
}

bool UNetworkingSubsystem::Signup(std::string UserId, std::string Password)
{
    if (!Connection.GetIsConnect())
    {
        UE_LOG(LogTemp, Log, TEXT("Is Not Connect"));
        return false;
    }

	UnrealEngineMessage::SignupRequest Signup;

	Signup.set_user_id(UserId);
	Signup.set_password(Password);

	return Send(Signup);
}

bool UNetworkingSubsystem::Register(std::string UserName)
{
    UnrealEngineMessage::RegisterRequest Register;

    Register.set_user_name(UserName);

    return Send(Register);
}

void UNetworkingSubsystem::CreateRoom(const FText& Text)
{
    UnrealEngineMessage::CreateRoomRequest CreateRoom;
    CreateRoom.set_room_name(TCHAR_TO_UTF8(*Text.ToString()));
    CreateRoom.set_max_player_count(3);

    Send(CreateRoom);
}

void UNetworkingSubsystem::FindRoom(const FText& Text)
{
    UnrealEngineMessage::FindRoomRequest FindRoom;
    FindRoom.set_room_name(TCHAR_TO_UTF8(*Text.ToString()));

    Send(FindRoom);
}

bool UNetworkingSubsystem::JoinRoom(uint32 RoomID)
{
    UnrealEngineMessage::JoinRoomRequest JoinRoom;

    JoinRoom.set_id(RoomID);

    return Send(JoinRoom);
}

bool UNetworkingSubsystem::LeaveRoom()
{
    UnrealEngineMessage::LeaveRoomRequest LeaveRoom;

    if(auto PC = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
    {
        if (auto Lobby = PC->GetLobbyBrowserWidget()) 
        {
            LeaveRoom.set_room_name(Lobby->GetSearchLobbyName());

            return Send(LeaveRoom);
        }
    }

    return false;
}

bool UNetworkingSubsystem::StartGame(std::string URL)
{
    UnrealEngineMessage::GameRequest Game;

    Game.set_url(URL);

    return Send(Game);
}

void UNetworkingSubsystem::Tick(float DeltaTime)
{
    if (RecvBufQueue.IsEmpty()) return;

    TUniquePtr<Buffer> RecvBuf;
    RecvBufQueue.Dequeue(RecvBuf);

    while (RecvBuf->GetStoredSize())
    {
        UE_LOG(LogTemp, Log, TEXT("Tick"));

        int written = BIO_write(Tls.GetRBIO(), RecvBuf->GetReadBuffer(), RecvBuf->GetStoredSize());
        if (written <= 0)
        {
            Tls.PrintSSLErr("BIO_write failed");
            UE_LOG(LogTemp, Log, TEXT("BIO_write failed"));
            Close();
            return;
        }
        else RecvBuf->OnRead(written);
    }

    if (Tls.GetHandshaking())
    {
        if (!TLSHandshake())
        {
            RecvBuf->Reset();
            return;
        }
    }

    Buffer Packet; int err;
    int n = Tls.GetRecvText(Packet, err);
    if (err == SSL_ERROR_NONE);
    else if (err == SSL_ERROR_WANT_WRITE)
    {
        // wbio에 뭔가 쌓였을 수 있음 -> flush
        Connection.FlushTLSWriteBuffer();
    }
    else
    {
        Tls.PrintSSLErr("SSL_read error");
    }
    // 핸드셰이크가 끝났다면 application data 처리

    if (n > 0)
    {
        if (RecvBuf->GetBufferSize() < n) RecvBuf->Resize(n);
        else RecvBuf->Reset();

        RecvBuf->Write(Packet.GetBuffer(), n);

        if (IsDataRemaining)
        {
            // 이전에 잘린 패킷이 있으면 이어붙임

            RemainingData->Append(MoveTemp(RecvBuf));
            // Append = 뒤에 붙이는 함수 (추가 구현 필요)

            if (ProcessPacket(RemainingData.Get()))
            {
                IsDataRemaining = false; // 끝까지 처리됨
            }
            // else: 여전히 잘림 상태, 다음 수신 대기
        }
        else if (!ProcessPacket(RecvBuf.Get()))
        {
             if (RemainingData->GetBufferSize() < RecvBuf->GetStoredSize()) RemainingData->Resize(RecvBuf->GetStoredSize());
             else RemainingData.Reset();

             RemainingData->Write(RecvBuf->GetBuffer(), RecvBuf->GetStoredSize());
             IsDataRemaining = true; // 잘린 데이터 보존
        }
    }
}

bool UNetworkingSubsystem::Send(const google::protobuf::Message& Message) 
{
    std::string serialized;
    Message.SerializeToString(&serialized);
    uint16_t msgId = htons(PacketHandler.GetMessageIdByName(Message.GetDescriptor()->full_name()));
    uint16_t packetLen = htons(sizeof(msgId) + serialized.size());

    Buffer buffer(sizeof(packetLen) + sizeof(msgId) + serialized.size());
    buffer.Write((uint8*)(&packetLen), sizeof(packetLen));
    buffer.Write((uint8*)(&msgId), sizeof(msgId));
    buffer.Write((uint8*)serialized.data(), serialized.size());

    int wret = SSL_write(Tls.GetSSL(), buffer.GetBuffer(), buffer.GetStoredSize());
    if (wret <= 0)
    {
        int err = SSL_get_error(Tls.GetSSL(), wret);
        if (err == SSL_ERROR_WANT_WRITE || err == SSL_ERROR_WANT_READ);
        else UE_LOG(LogTemp, Error, TEXT("SSL_write error: %d"), err);
    }

    Connection.WakeEventTrigger();

    return Connection.FlushTLSWriteBuffer();
}

bool UNetworkingSubsystem::ProcessPacket(Buffer* buffer)
{
    size_t offset = 0;
    const uint8* data = buffer->GetReadBuffer();
    size_t totalLen = buffer->GetStoredSize();

    while (offset + sizeof(uint16_t) <= totalLen)
    {
        uint16_t packetLen = 0;
        memcpy(&packetLen, data + offset, sizeof(uint16_t));
        packetLen = ntohs(packetLen);

        if (offset + sizeof(uint16_t) + packetLen > totalLen)
            break; // incomplete packet

        const uint8* packetStart = data + offset + sizeof(uint16_t);
        HandlePacket(packetStart, packetLen);
        offset += sizeof(uint16_t) + packetLen;
    }

    if (offset < totalLen)
    {
        buffer->PreserveFrom(offset);
        return false;
    }

    buffer->Reset();
    return true;
}

void UNetworkingSubsystem::HandlePacket(const uint8* data, size_t len)
{
    uint16_t msgId;
    memcpy(&msgId, data, sizeof(msgId));
    msgId = ntohs(msgId);

    auto message = PacketHandler.CreateMessageById(msgId);
    if (!message)
        return;

    if (!message->ParseFromArray(data + sizeof(msgId), len - sizeof(msgId)))
        return;

    UE_LOG(LogTemp, Log, TEXT("Handle Packet message Type: %s"), *FString(message->GetDescriptor()->full_name().c_str()));

    PacketHandler.HandlePacket(*this, msgId, *message);

}