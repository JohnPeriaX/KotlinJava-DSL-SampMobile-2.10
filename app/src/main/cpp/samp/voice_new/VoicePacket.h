#pragma once

#include "include/util/Memory.hpp"

#pragma pack(push, 1)

struct VoicePacket
{
    uint32_t hash;
    uint32_t svrkey;
    uint32_t packet;
    uint32_t stream;
    uint16_t sender;
    uint16_t length;
    uint32_t packid;
    uint8_t data[];

    uint32_t GetFullSize();
    bool CheckHeader();
    void CalcHash();
};

#pragma pack(pop)

static_assert(offsetof(VoicePacket, hash) == 0, "[VoicePacket] : 'hash' "
    "field should be located at beginning of packet struct");

using VoicePacketContainer = Memory::ObjectContainer<VoicePacket>;
using VoicePacketContainerPtr = Memory::ObjectContainerPtr<VoicePacket>;
#define MakeVoicePacketContainer MakeObjectContainer(VoicePacket)