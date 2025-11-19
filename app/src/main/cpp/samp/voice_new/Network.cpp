#include "../main.h"
#include "../game/game.h"
#include "../net/netgame.h"

#include "Network.h"

extern CNetGame *pNetGame;

bool Network::Init() noexcept
{
    if(Network::initStatus)
        return false;

    LogVoice("[sv:dbg:network:init] : module initializing...");

    Network::socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(Network::socketHandle == INVALID_SOCKET) return false;

    if (const int sendBufferSize { kSendBufferSize }, recvBufferSize { kRecvBufferSize };
        setsockopt(Network::socketHandle, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const void*>
            (&sendBufferSize), sizeof(sendBufferSize)) == SOCKET_ERROR ||
        setsockopt(Network::socketHandle, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const void*>
            (&recvBufferSize), sizeof(recvBufferSize)) == SOCKET_ERROR)
    {
        return false;
    }

    // call the function so no need raknet file
    /*RakNet::AddConnectCallback(Network::OnRaknetConnect);
    RakNet::AddReceiveCallback(Network::OnRaknetReceive);
    RakNet::AddRpcCallback(Network::OnRaknetRpc);
    RakNet::AddDisconnectCallback(Network::OnRaknetDisconnect);*/

    Network::connectionStatus = ConnectionStatus::Disconnected;

    LogVoice("[sv:dbg:network:init] : module initialized");

    Network::initStatus = true;

    return true;
}

void Network::Free() noexcept
{
    if(!Network::initStatus)
        return;

    LogVoice("[sv:dbg:network:free] : module releasing...");

    if(Network::connectionStatus != ConnectionStatus::Disconnected)
    {
        for(const auto& disconnectCallback : Network::disconnectCallbacks)
        {
            if(disconnectCallback != nullptr) 
                disconnectCallback();
        }
    }

    Network::connectionStatus = ConnectionStatus::Disconnected;

    if(Network::voiceThread.joinable())
        Network::voiceThread.detach();

    close(Network::socketHandle);
    Network::socketHandle = INVALID_SOCKET;

    Network::serverIp.clear();
    Network::serverKey = NULL;

    /*ZeroMemory(Network::inputVoicePacket.GetData(),  Network::inputVoicePacket.GetSize());
    ZeroMemory(Network::outputVoicePacket.GetData(), Network::outputVoicePacket.GetSize());*/
    memset(Network::inputVoicePacket.GetData(), 0, Network::inputVoicePacket.GetSize());
    memset(Network::outputVoicePacket.GetData(), 0, Network::outputVoicePacket.GetSize());

    while(!Network::controlQueue.empty())
        Network::controlQueue.pop();
    while(!Network::voiceQueue.empty())
        Network::voiceQueue.pop();

    LogVoice("[sv:dbg:network:free] : module released");

    Network::initStatus = false;
}

bool Network::SendControlPacket(const uint16_t packet, const void *dataAddr, const uint16_t dataSize) noexcept
{
    if(Network::connectionStatus != ConnectionStatus::Connected)
        return false;

    RakNet::BitStream bsSend;

    bsSend.Write<uint8_t>(kRaknetPacketId);

    bsSend.Write<uint16_t>(packet);
    bsSend.Write<uint16_t>(dataSize);

    if(dataAddr != nullptr && dataSize != 0)
    {
        bsSend.Write(static_cast<const char *>(dataAddr), dataSize);
    }

    return pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}

bool Network::SendVoicePacket(const void *dataAddr, const uint16_t dataSize) noexcept
{
    if(dataAddr == nullptr || dataSize == 0 || dataSize > kMaxVoiceDataSize)
        return false;

    if(Network::connectionStatus != ConnectionStatus::Connected)
        return false;

    Network::outputVoicePacket->length = dataSize;
    Network::outputVoicePacket->CalcHash();

    memcpy(Network::outputVoicePacket->data, dataAddr, dataSize);

    const auto voicePacketAddr = reinterpret_cast<const char*>(&Network::outputVoicePacket);
    const auto voicePacketSize = static_cast<int>(Network::outputVoicePacket->GetFullSize());

    const auto sended = send(Network::socketHandle, voicePacketAddr, voicePacketSize, NULL);

    ++Network::outputVoicePacket->packid;

    return sended == voicePacketSize;
}

void Network::EndSequence() noexcept
{
    if (!Network::initStatus)
        return;

    Network::outputVoicePacket->packid = NULL;
}

ControlPacketContainerPtr Network::ReceiveControlPacket() noexcept
{
    if(!Network::initStatus || Network::controlQueue.empty())
        return nullptr;

    auto controlPacket = std::move(*Network::controlQueue.front());
    Network::controlQueue.pop();

    return controlPacket;
}

VoicePacketContainerPtr Network::ReceiveVoicePacket() noexcept
{
    if(!Network::initStatus || Network::voiceQueue.empty())
        return nullptr;

    auto voicePacket = std::move(*Network::voiceQueue.front());
    Network::voiceQueue.pop();

    return voicePacket;
}

std::size_t Network::AddConnectCallback(ConnectCallback callback) noexcept
{
    if(!Network::initStatus)  return -1;

    for(std::size_t i { 0 }; i < Network::connectCallbacks.size(); ++i)
    {
        if(Network::connectCallbacks[i] == nullptr)
        {
            Network::connectCallbacks[i] = std::move(callback);
            return i;
        }
    }

    Network::connectCallbacks.emplace_back(std::move(callback));
    return Network::connectCallbacks.size() - 1;
}

std::size_t Network::AddSvConnectCallback(SvConnectCallback callback) noexcept
{
    if(!Network::initStatus) return -1;

    for(std::size_t i { 0 }; i < Network::svConnectCallbacks.size(); ++i)
    {
        if(Network::svConnectCallbacks[i] == nullptr)
        {
            Network::svConnectCallbacks[i] = std::move(callback);
            return i;
        }
    }

    Network::svConnectCallbacks.emplace_back(std::move(callback));
    return Network::svConnectCallbacks.size() - 1;
}

std::size_t Network::AddSvInitCallback(SvInitCallback callback) noexcept
{
    if(!Network::initStatus) return -1;

    for(std::size_t i { 0 }; i < Network::svInitCallbacks.size(); ++i)
    {
        if(Network::svInitCallbacks[i] == nullptr)
        {
            Network::svInitCallbacks[i] = std::move(callback);
            return i;
        }
    }

    Network::svInitCallbacks.emplace_back(std::move(callback));
    return Network::svInitCallbacks.size() - 1;
}

std::size_t Network::AddDisconnectCallback(DisconnectCallback callback) noexcept
{
    if(!Network::initStatus) return -1;

    for(std::size_t i { 0 }; i < Network::disconnectCallbacks.size(); ++i)
    {
        if(Network::disconnectCallbacks[i] == nullptr)
        {
            Network::disconnectCallbacks[i] = std::move(callback);
            return i;
        }
    }

    Network::disconnectCallbacks.emplace_back(std::move(callback));
    return Network::disconnectCallbacks.size() - 1;
}

void Network::RemoveConnectCallback(const std::size_t callback) noexcept
{
    if(!Network::initStatus)
        return;

    if(callback >= Network::connectCallbacks.size())
        return;

    Network::connectCallbacks[callback] = nullptr;
}

void Network::RemoveSvConnectCallback(const std::size_t callback) noexcept
{
    if(!Network::initStatus)
        return;

    if(callback >= Network::svConnectCallbacks.size())
        return;

    Network::svConnectCallbacks[callback] = nullptr;
}

void Network::RemoveSvInitCallback(const std::size_t callback) noexcept
{
    if(!Network::initStatus)
        return;

    if(callback >= Network::svInitCallbacks.size())
        return;

    Network::svInitCallbacks[callback] = nullptr;
}

void Network::RemoveDisconnectCallback(const std::size_t callback) noexcept
{
    if(!Network::initStatus)
        return;

    if(callback >= Network::disconnectCallbacks.size())
        return;

    Network::disconnectCallbacks[callback] = nullptr;
}

void Network::VoiceThread() noexcept
{
    VoicePacket keepAlivePacket;
    Timer::time_t keepAliveLastTime { NULL };

    //ZeroMemory(&keepAlivePacket, sizeof(keepAlivePacket));
    memset(&keepAlivePacket, 0, sizeof(keepAlivePacket));

    keepAlivePacket.svrkey = Network::serverKey;
    keepAlivePacket.packet = SV::VoicePacketType::keepAlive;
    keepAlivePacket.packid = NULL;
    keepAlivePacket.length = NULL;
    keepAlivePacket.sender = NULL;
    keepAlivePacket.stream = NULL;
    keepAlivePacket.CalcHash();

    while(true)
    {
        const auto curStatus = Network::connectionStatus;

        if(curStatus == ConnectionStatus::Disconnected) break;

        // Sending keep-alive packets...
        // -----------------------------------------------------------------

        const auto curTime = Timer::Get();

        if(curTime - keepAliveLastTime >= kKeepAliveInterval)
        {
            send(Network::socketHandle, reinterpret_cast<const char *>(&keepAlivePacket),
                 sizeof(keepAlivePacket), NULL);

            keepAliveLastTime = curTime;
        }

        // Receiving voice packets...
        // -----------------------------------------------------------------

        if(curStatus != ConnectionStatus::Connected)
        {
            SleepForMilliseconds(100);
            continue;
        }

        const auto received = recv(Network::socketHandle,
            static_cast<char*>(Network::inputVoicePacket.GetData()),
            Network::inputVoicePacket.GetSize(), NULL);

        if(received == SOCKET_ERROR)
            break;

        if(received < static_cast<decltype(received)>(sizeof(VoicePacket)))
            continue;

        if(!Network::inputVoicePacket->CheckHeader()) continue;
        if(received != Network::inputVoicePacket->GetFullSize()) continue;
        if(Network::inputVoicePacket->packet == SV::VoicePacketType::keepAlive) continue;
        
        Network::voiceQueue.try_emplace(MakeVoicePacketContainer(Network::inputVoicePacket));
    }
}

void Network::OnRaknetConnect(const char *ip, const uint32_t port) noexcept
{
    if(!Network::initStatus)
        return;

    LogVoice("[dbg:raknet:client:connect] : connecting to game server '*.*.*.*:****'...", ip, port);

    Network::serverIp = ip;

    if(Network::connectionStatus == ConnectionStatus::Disconnected)
    {
        for(const auto& connectCallback : Network::connectCallbacks)
        {
            if(connectCallback != nullptr) 
                connectCallback(Network::serverIp, port);
        }
    }

    Network::connectionStatus = ConnectionStatus::RNConnecting;

    LogVoice("[dbg:raknet:client:connect] : connected");
}

bool Network::OnRaknetRpc(const int id, RakNet::BitStream& parameters) noexcept
{
    if(!Network::initStatus || id != kRaknetConnectRcpId)
        return true;

    if(Network::svConnectCallbacks.empty())
        return true;

    SV::ConnectPacket stData {};

    for(const auto& svConnectCallback : Network::svConnectCallbacks)
    {
        if(svConnectCallback != nullptr) 
            svConnectCallback(stData);
    }

    parameters.Write(reinterpret_cast<const char*>(&stData), sizeof(stData));

    LogVoice("[sv:dbg:network:connect] : raknet connecting... "
        "(version:%hhu;micro:%hhu)", stData.version, stData.micro);

    return true;
}

bool Network::OnRaknetReceive(Packet& packet) noexcept
{
    if(!Network::initStatus)
        return true;

    if(packet.length < sizeof(uint8_t) + sizeof(ControlPacket))
        return true;

    if(*packet.data != kRaknetPacketId)
        return true;

    const auto controlPacketPtr = reinterpret_cast<ControlPacket*>(packet.data + sizeof(uint8_t));
    const uint32_t controlPacketSize = packet.length - sizeof(uint8_t);

    if(controlPacketSize != controlPacketPtr->GetFullSize())
        return false;

    switch(controlPacketPtr->packet)
    {
        case SV::ControlPacketType::serverInfo:
        {
            const auto& stData = *reinterpret_cast<SV::ServerInfoPacket*>(controlPacketPtr->data);
            if(controlPacketPtr->length != sizeof(stData)) return false;

            LogVoice("[sv:dbg:network:serverInfo] : connecting to voiceserver "
                "'*.*.*.*:%hu'...", Network::serverIp.c_str(), stData.serverPort);

            sockaddr_in serverAddress {};

            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = inet_addr(Network::serverIp.c_str());
            serverAddress.sin_port = htons(stData.serverPort);

            if (connect(Network::socketHandle, reinterpret_cast<const sockaddr*>(&serverAddress),
                sizeof(serverAddress)) == SOCKET_ERROR)
            {
                LogVoice("[sv:err:network:serverInfo] : connect error.");
                return false;
            }

            Network::serverKey = stData.serverKey;

            Network::outputVoicePacket->svrkey = Network::serverKey;
            Network::outputVoicePacket->packet = SV::VoicePacketType::voicePacket;
            Network::outputVoicePacket->packid = NULL;
            Network::outputVoicePacket->length = NULL;
            Network::outputVoicePacket->sender = NULL;
            Network::outputVoicePacket->stream = NULL;
            Network::outputVoicePacket->CalcHash();

            Network::connectionStatus = ConnectionStatus::SVConnecting;

            Network::voiceThread = std::thread(Network::VoiceThread);
        } 
        break;
        case SV::ControlPacketType::pluginInit:
        {
            const auto& stData = *reinterpret_cast<SV::PluginInitPacket*>(controlPacketPtr->data);
            if(controlPacketPtr->length != sizeof(stData)) return false;

            LogVoice("[sv:dbg:network:pluginInit] : plugin init packet "
                "(bitrate:%u;mute:%hhu)", stData.bitrate, stData.mute);

            for(const auto& svInitCallback : Network::svInitCallbacks)
            {
                if(svInitCallback != nullptr) 
                    svInitCallback(stData);
            }

            Network::connectionStatus = ConnectionStatus::Connected;
        } 
        break;
        default:
        {
            Network::controlQueue.try_emplace(MakeControlPacketContainer(controlPacketPtr, controlPacketSize));
        }
    }

    return false;
}

void Network::OnRaknetDisconnect() noexcept
{
    if(!Network::initStatus)
        return;

    LogVoice("[sv:dbg:network:disconnect] : raknet disconnected");

    if(Network::connectionStatus != ConnectionStatus::Disconnected)
    {
        for(const auto& disconnectCallback : Network::disconnectCallbacks)
        {
            if(disconnectCallback != nullptr) 
                disconnectCallback();
        }
    }

    Network::connectionStatus = ConnectionStatus::Disconnected;

    if(Network::voiceThread.joinable())
        Network::voiceThread.detach();

    Network::serverIp.clear();
    Network::serverKey = NULL;

    /*ZeroMemory(Network::inputVoicePacket.GetData(), Network::inputVoicePacket.GetSize());
    ZeroMemory(Network::outputVoicePacket.GetData(), Network::outputVoicePacket.GetSize());*/
    memset(Network::inputVoicePacket.GetData(), 0, Network::inputVoicePacket.GetSize());
    memset(Network::outputVoicePacket.GetData(), 0, Network::outputVoicePacket.GetSize());

    while (!Network::controlQueue.empty())
        Network::controlQueue.pop();
    while (!Network::voiceQueue.empty())
        Network::voiceQueue.pop();
}

bool Network::initStatus { false };

int Network::socketHandle { INVALID_SOCKET };
int Network::connectionStatus { ConnectionStatus::Disconnected };
std::thread Network::voiceThread;
std::string Network::serverIp;
DWORD Network::serverKey { NULL };

std::vector<Network::ConnectCallback> Network::connectCallbacks;
std::vector<Network::SvConnectCallback> Network::svConnectCallbacks;
std::vector<Network::SvInitCallback> Network::svInitCallbacks;
std::vector<Network::DisconnectCallback> Network::disconnectCallbacks;

SPSCQueue<ControlPacketContainerPtr> Network::controlQueue { 128 };
SPSCQueue<VoicePacketContainerPtr> Network::voiceQueue { 512 };

VoicePacketContainer Network::inputVoicePacket { kMaxVoiceDataSize };
VoicePacketContainer Network::outputVoicePacket { kMaxVoiceDataSize };