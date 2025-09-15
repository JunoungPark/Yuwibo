#include "FPacketHandler.h"
#include "NetworkingSubsystem.h"
#include <iostream>

void FPacketHandler::Init()
{
    _packetTable.SetNum(19);

    Register(
        0,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::Ping(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });

    Register(
        1,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::Pong(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
            const auto& pkt = static_cast<const UnrealEngineMessage::Pong&>(message);
            std::cout << "Pong received: " << pkt.timestamp() << std::endl;

        });

    Register(
        2,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::SigninRequest(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });


    Register(
        3,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::SigninResponse(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
            const auto& pkt = static_cast<const UnrealEngineMessage::SigninResponse&>(message);

            network.OnSigninResponse.ExecuteIfBound(pkt.success(), pkt.error_code());
        });

    Register(
        4,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::SignupRequest(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });

    Register(
        5,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::SignupResponse(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
            const auto& pkt = static_cast<const UnrealEngineMessage::SignupResponse&>(message);

            network.OnSignupResponse.ExecuteIfBound(pkt.success(), pkt.error_code());
        });

    Register(
        6,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::RegisterRequest(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });

    Register(
        7,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::RegisterResponse(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
            network.OnSigninResponse.ExecuteIfBound(true, UnrealEngineMessage::ErrorCode::NONE);
        });
    Register(
        8,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::CreateRoomRequest(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });

    Register(
        9,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::FindRoomRequest(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });

    Register(
        10,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::FindRoomResponse(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
            const auto& pkt = static_cast<const UnrealEngineMessage::FindRoomResponse&>(message);
            
            network.OnFindRoomResponse.ExecuteIfBound(pkt.rooms());
        });

    Register(
        11,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::JoinRoomRequest(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });

    Register(
        12,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::JoinRoomResponse(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
            const auto& pkt = static_cast<const UnrealEngineMessage::JoinRoomResponse&>(message);

            network.OnJoinRoomResponse.ExecuteIfBound(pkt.success(), pkt.error_code(), pkt.room(), pkt.users(), pkt.my_id());
        });

    Register(
        13,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::LeaveRoomRequest(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });

    Register(
        14,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::UpdateRoomInfo(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
            const auto& pkt = static_cast<const UnrealEngineMessage::UpdateRoomInfo&>(message);

            network.OnUpdateRoomInfo.ExecuteIfBound(pkt.room(), pkt.users(), pkt.my_id());
        });

    Register(
        15,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::BroadcastRequest(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });

    Register(
        16,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::BroadcastResponse(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
            const auto& pkt = static_cast<const UnrealEngineMessage::BroadcastResponse&>(message);

            network.OnBroadcastResponse.ExecuteIfBound(pkt.userinfo(), pkt.message());
        });

    Register(
        17,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::GameRequest(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
        });

    Register(
        18,
        []() -> google::protobuf::Message*
        { return new UnrealEngineMessage::GameResponse(); },
        [](UNetworkingSubsystem& network, const google::protobuf::Message& message)
        {
            const auto& pkt = static_cast<const UnrealEngineMessage::GameResponse&>(message);

            network.OnGameResponse.ExecuteIfBound(pkt.url());
        });

}

void FPacketHandler::Register(uint16_t id, std::function<google::protobuf::Message* ()> creator, PacketHandlerFunc handler)
{
    _packetTable[id] = MakeUnique<PacketEntry>(creator, handler);

    // 타입 이름을 가져오기 위해 임시로 메시지 생성
    TUniquePtr<google::protobuf::Message> temp(creator());
    std::string typeName = temp->GetDescriptor()->full_name();
;
    _nameToId[typeName] = id;

    FString fTypeName(typeName.c_str()); // std::string -> FString
    UE_LOG(LogTemp, Log, TEXT("Registered message : %s with ID: %d"), *fTypeName, id);
}

google::protobuf::Message* FPacketHandler::CreateMessageById(uint16_t id)
{
    if (id >= _packetTable.Num())
    {
        std::cerr << "CreateMessageById: Invalid ID (out of range): " << id << std::endl;
        return nullptr;
    }

    if (auto& creator = _packetTable[id]->_creator)
        return creator();

    std::cerr << "CreateMessageById: Unknown ID: " << id << std::endl;
    return nullptr;
}

uint16_t FPacketHandler::GetMessageIdByName(const std::string& typeName)
{

    auto it = _nameToId.find(typeName);
    if (it != _nameToId.end())
    {
        return it->second;
    }
    std::cerr << "GetMessageIdByName: Unknown type name: " << typeName << std::endl;
    return 0;
}

void FPacketHandler::HandlePacket(UNetworkingSubsystem& network, uint16_t packetId, const google::protobuf::Message& message)
{

    if (packetId >= _packetTable.Num())
    {
        std::cerr << "CreateMessageById: Invalid ID (out of range): " << packetId << std::endl;
        return;
    }

    if (auto& handler = _packetTable[packetId]->_handler)
        return handler(network, message);

    std::cerr << "HandlePacket: Unknown packet ID: " << packetId << std::endl;
}