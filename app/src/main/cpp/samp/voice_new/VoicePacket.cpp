#include "VoicePacket.h"

static uint32_t CalcCrc32cHash(const char *buffer, uint32_t length, uint32_t crc = 0) noexcept
{
    crc = ~crc;

    while (length--)
    {
        crc ^= *buffer++;

        for (int k = 0; k < 8; ++k)
        {
            crc = crc & 1 ? (crc >> 1) ^ 0x82f63b78 : crc >> 1;
        }
    }

    return ~crc;
}

uint32_t VoicePacket::GetFullSize()
{
    return sizeof(*this) + this->length;
}

bool VoicePacket::CheckHeader()
{
    return this->hash == CalcCrc32cHash(
        (char*)(this) + sizeof(this->hash),
        sizeof(*this) - sizeof(this->hash)
    );
}

void VoicePacket::CalcHash()
{
    this->hash = CalcCrc32cHash(
        (char*)(this) + sizeof(this->hash),
        sizeof(*this) - sizeof(this->hash)
    );
}
