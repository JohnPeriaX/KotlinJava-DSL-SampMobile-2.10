#include "LocalStream.h"

LocalStream::LocalStream(const StreamType type, const uint32_t color,
                         std::string name, const float distance) noexcept
    : Stream(BASS_SAMPLE_MUTEMAX, type, color, std::move(name), distance)
    , distance(distance)
{}

void LocalStream::SetDistance(const float distance) noexcept
{
    this->distance = distance;

    for(const auto& channel : this->GetChannels())
    {
    }
}

void LocalStream::OnChannelCreate(const Channel& channel) noexcept
{
    this->Stream::OnChannelCreate(channel);
}
