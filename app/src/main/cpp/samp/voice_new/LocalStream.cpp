#include "LocalStream.h"

LocalStream::LocalStream(const StreamType type, const uint32_t color,
                         std::string name, const float distance) noexcept
    : Stream(BASS_SAMPLE_MUTEMAX | BASS_SAMPLE_3D, type, color, std::move(name), distance)
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

    // Configure 3D attributes based on stream distance
    const float maxdist = this->distance > 0.0f ? this->distance : 50.0f;
    const float mindist = maxdist * 0.1f; // gentle falloff near the source
    BASS_ChannelSet3DAttributes(channel.GetHandle(), BASS_3DMODE_NORMAL,
        mindist, maxdist, 360, 360, -1.0f);
}
