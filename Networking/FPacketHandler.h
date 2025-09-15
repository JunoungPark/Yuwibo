#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "FTLSConnection.h"

using PacketHandlerFunc = std::function<void(class UNetworkingSubsystem&, const google::protobuf::Message&)>;

struct PacketEntry {

    PacketEntry(std::function<google::protobuf::Message* ()> creator, PacketHandlerFunc handler) : _creator(creator), _handler(handler) {}

    std::function<google::protobuf::Message* ()> _creator;
    PacketHandlerFunc _handler;

};

class FPacketHandler {

public:
    FPacketHandler() {}
    ~FPacketHandler() {}

    FPacketHandler(const FPacketHandler&) = delete;
    FPacketHandler& operator=(const FPacketHandler&) = delete;

    FPacketHandler(FPacketHandler&&) = default;
    FPacketHandler& operator=(FPacketHandler&&) = default;
    void Init();
    void Register(uint16_t id, std::function<google::protobuf::Message* ()> creator, PacketHandlerFunc handler);

    // 메시지 ID로 메시지 생성
    google::protobuf::Message* CreateMessageById(uint16_t id);

    // 타입 이름으로 메시지 ID 반환
    uint16_t GetMessageIdByName(const std::string& typeName);
    void HandlePacket(UNetworkingSubsystem& network, uint16_t packetId, const google::protobuf::Message& message);

private:

    TArray<TUniquePtr<PacketEntry>> _packetTable;
    std::unordered_map<std::string, uint16_t> _nameToId;

};