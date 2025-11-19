#pragma once

#include "../vendor/raknet/BitStream.h"
#include "../vendor/raknet/RakClient.h"

#include "include/SPSCQueue.h"

#include "include/util/Timer.h"

#include "ControlPacket.h"
#include "VoicePacket.h"
#include "Header.h"

class Network {
    Network() = delete;
    ~Network() = delete;
    Network(const Network&) = delete;
    Network(Network&&) = delete;
    Network& operator=(const Network&) = delete;
    Network& operator=(Network&&) = delete;

public:
    static constexpr uint8_t kRaknetPacketId = 222;
    static constexpr int kRaknetConnectRcpId = 25;
    static constexpr uint32_t kMaxVoicePacketSize = 1400;
    static constexpr uint32_t kMaxVoiceDataSize = kMaxVoicePacketSize - sizeof(VoicePacket);
    static constexpr uint32_t kRecvBufferSize = 2 * 1024 * 1024;
    static constexpr uint32_t kSendBufferSize = 64 * 1024;
    static constexpr Timer::time_t kKeepAliveInterval = 2000;

private:
    using ConnectCallback = std::function<void(const std::string&, uint16_t)>;
    using SvConnectCallback = std::function<void(SV::ConnectPacket&)>;
    using SvInitCallback = std::function<bool(const SV::PluginInitPacket&)>;
    using DisconnectCallback = std::function<void()>;

private:
    struct ConnectionStatus
    {
        enum
        {
            Disconnected,
            RNConnecting,
            SVConnecting,
            Connected
        };
    };

public:
    static bool Init() noexcept;
    static void Free() noexcept;

    static bool SendControlPacket(uint16_t packet, const void *dataAddr = nullptr, uint16_t dataSize = 0) noexcept;
    static bool SendVoicePacket(const void *dataAddr, uint16_t dataSize) noexcept;
    static void EndSequence() noexcept;
    static ControlPacketContainerPtr ReceiveControlPacket() noexcept;
    static VoicePacketContainerPtr ReceiveVoicePacket() noexcept;

    static std::size_t AddConnectCallback(ConnectCallback callback) noexcept;
    static std::size_t AddSvConnectCallback(SvConnectCallback callback) noexcept;
    static std::size_t AddSvInitCallback(SvInitCallback callback) noexcept;
    static std::size_t AddDisconnectCallback(DisconnectCallback callback) noexcept;
    static void RemoveConnectCallback(std::size_t callback) noexcept;
    static void RemoveSvConnectCallback(std::size_t callback) noexcept;
    static void RemoveSvInitCallback(std::size_t callback) noexcept;
    static void RemoveDisconnectCallback(std::size_t callback) noexcept;

    static void VoiceThread() noexcept;
    
    static void OnRaknetConnect(const char *ip, uint32_t port) noexcept;
    static bool OnRaknetRpc(int id, RakNet::BitStream& parameters) noexcept;
    static bool OnRaknetReceive(Packet& packet) noexcept;
    static void OnRaknetDisconnect() noexcept;

private:
    static bool initStatus;

    static int socketHandle;
    static int connectionStatus;
    static std::thread voiceThread;
    static std::string serverIp;
    static uint32_t serverKey;

    static std::vector<ConnectCallback> connectCallbacks;
    static std::vector<SvConnectCallback> svConnectCallbacks;
    static std::vector<SvInitCallback> svInitCallbacks;
    static std::vector<DisconnectCallback> disconnectCallbacks;

    static SPSCQueue<ControlPacketContainerPtr> controlQueue;
    static SPSCQueue<VoicePacketContainerPtr> voiceQueue;

    static VoicePacketContainer inputVoicePacket;
    static VoicePacketContainer outputVoicePacket;
};