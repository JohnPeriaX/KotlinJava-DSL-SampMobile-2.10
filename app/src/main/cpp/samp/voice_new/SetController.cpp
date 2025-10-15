#include "SetController.h"
#include "main.h"

SetController::SetController(const uint32_t parameter, const float value) noexcept
    : Parameter(parameter), value(value) {}

void SetController::Apply(const Channel& channel) const noexcept
{
    BASS_ChannelSetAttribute(channel.GetHandle(), this->parameter, this->value);
}
