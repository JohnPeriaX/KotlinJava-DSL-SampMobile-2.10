#include "Stream.h"

#include "SetController.h"
#include "SlideController.h"

Stream::Stream(const uint32_t streamFlags, const StreamType type,
               const uint32_t color, std::string name, const float distance) noexcept
    : streamFlags(streamFlags)
    , streamInfo(type, color, std::move(name), distance)
{}

const StreamInfo& Stream::GetInfo() const noexcept
{
    return this->streamInfo;
}

void Stream::Tick() noexcept
{
    for(const auto& channel : this->channels)
    {
        if(channel->channelplay) 
        {
            channel->channelplay = false;
            this->OnChannelPlay(*channel);
        }
        else if(channel->channelstop) 
        {
            channel->channelstop = false;
            this->OnChannelStop(*channel);
        }

        if(channel->HasSpeaker() && !channel->IsActive())
        {
            if(channel->playing) this->OnChannelStop(*channel);
            channel->Reset();
        }
    }
}

void Stream::Push(const VoicePacket& packet)
{
    const ChannelPtr* channelPtr { nullptr };

    for(const auto& channel : this->channels)
    {
        if(channel->GetSpeaker() == packet.sender)
        {
            channelPtr = &channel;
            break;
        }
    }

    if(channelPtr == nullptr)
    {
        for(const auto& channel : this->channels)
        {
            if(!channel->HasSpeaker())
            {
                LogVoice("[sv:dbg:stream:push] : channel %p was occupied by player %hu "
                    "(stream:%s)", channel.get(), packet.sender, this->streamInfo.GetName().c_str());

                channel->SetSpeaker(packet.sender);

                channelPtr = &channel;
                break;
            }
        }
    }

    if(channelPtr == nullptr)
    {
        this->channels.emplace_back(MakeChannel(this->streamFlags));

        int channelCount = 1;
        for(const auto& channel : this->channels)
        {
            if(channelCount == this->channels.size())
            {
                //channel->SetPlayCallback(std::bind(&Stream::OnChannelPlay, this, std::placeholders::_1));
                //channel->SetStopCallback(std::bind(&Stream::OnChannelStop, this, std::placeholders::_1));
                channel->SetSpeaker(packet.sender);

                LogVoice("[sv:dbg:stream:push] : channel %p for player %hu created (stream:%s)",
                    channel.get(), packet.sender, this->streamInfo.GetName().c_str());

                this->OnChannelCreate(*channel);

                channelPtr = &channel;
                break;
            }

            channelCount++;
        }
    }

    if(channelPtr) 
        (*channelPtr)->Push(packet.packid, packet.data, packet.length);
}

void Stream::Reset() noexcept
{
    for(const auto& channel : this->channels)
    {
        if(channel->playing) this->OnChannelStop(*channel);
        channel->Reset();
    }
}

void Stream::SetParameter(const uint8_t parameter, const float value)
{
    const auto& parameterPtr = this->parameters[parameter] =
        MakeSetController(parameter, value);

    for(const auto& channel : this->channels)
    {
        parameterPtr->Apply(*channel);
    }
}

void Stream::SlideParameter(const uint8_t parameter, const float startValue,
                            const float endValue, const uint32_t time)
{
    const auto& parameterPtr = this->parameters[parameter] =
        MakeSlideController(parameter, startValue, endValue, time);

    for(const auto& channel : this->channels)
    {
        parameterPtr->Apply(*channel);
    }
}

void Stream::EffectCreate(const uint32_t effect, const uint32_t number, const int priority,
                          const void* const paramPtr, const uint32_t paramSize)
{
    const auto& effectPtr = this->effects[effect] =
        MakeEffect(number, priority, paramPtr, paramSize);

    for(const auto& channel : this->channels)
    {
        effectPtr->Apply(*channel);
    }
}

void Stream::EffectDelete(const uint32_t effect)
{
    this->effects.erase(effect);
}

std::size_t Stream::AddPlayCallback(PlayCallback playCallback)
{
    for(std::size_t i { 0 }; i < this->playCallbacks.size(); ++i)
    {
        if(this->playCallbacks[i] == nullptr)
        {
            this->playCallbacks[i] = std::move(playCallback);
            return i;
        }
    }

    this->playCallbacks.emplace_back(std::move(playCallback));
    return this->playCallbacks.size() - 1;
}

std::size_t Stream::AddStopCallback(StopCallback stopCallback)
{
    for(std::size_t i { 0 }; i < this->stopCallbacks.size(); ++i)
    {
        if(this->stopCallbacks[i] == nullptr)
        {
            this->stopCallbacks[i] = std::move(stopCallback);
            return i;
        }
    }

    this->stopCallbacks.emplace_back(std::move(stopCallback));
    return this->stopCallbacks.size() - 1;
}

void Stream::RemovePlayCallback(const std::size_t callback) noexcept
{
    if(callback >= this->playCallbacks.size())
        return;

    this->playCallbacks[callback] = nullptr;
}

void Stream::RemoveStopCallback(const std::size_t callback) noexcept
{
    if(callback >= this->stopCallbacks.size())
        return;

    this->stopCallbacks[callback] = nullptr;
}

void Stream::OnChannelCreate(const Channel& channel)
{
    BASS_ChannelSetAttribute(channel.GetHandle(), BASS_ATTRIB_VOL, 4.f);

    for(const auto& parameter : this->parameters)
    {
        parameter.second->Apply(channel);
    }

    for(const auto& effect : this->effects)
    {
        effect.second->Apply(channel);
    }
}

void Stream::OnChannelPlay(const Channel& channel) noexcept
{
    if(channel.HasSpeaker())
    {
        for(const auto& playCallback : this->playCallbacks)
        {
            if(playCallback != nullptr) 
                playCallback(*this, channel.GetSpeaker());
        }
    }
}

void Stream::OnChannelStop(const Channel& channel) noexcept
{
    if(channel.HasSpeaker())
    {
        for(const auto& stopCallback : this->stopCallbacks)
        {
            if(stopCallback != nullptr) 
                stopCallback(*this, channel.GetSpeaker());
        }
    }
}

const std::vector<ChannelPtr>& Stream::GetChannels() const noexcept
{
    return this->channels;
}
