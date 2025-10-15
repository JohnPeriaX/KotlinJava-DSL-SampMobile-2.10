#include "ControlPacket.h"

uint32_t ControlPacket::GetFullSize()
{
    return sizeof(*this) + this->length;
}
