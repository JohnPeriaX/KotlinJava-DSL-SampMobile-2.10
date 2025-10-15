#include "Channel.h"

#include "PluginConfig.h"
#include "main.h"

Channel::Channel(const uint32_t channelFlags)
            : handle(BASS_StreamCreate(SV::kFrequency1, 1, channelFlags, STREAMPROC_PUSH, nullptr))
            , decoder(opus_decoder_create(SV::kFrequency, 1, &opusErrorCode))
    {
    //  BASS_ChannelSetAttribute(handle, BASS_ATTRIB_VOL, 1.0f);

    if(this->handle == NULL) FLog("[sv:err:channel] : "
                                 "failed to create bass channel (code:%d)", BASS_ErrorGetCode());
    if(this->decoder == nullptr) FLog("[sv:err:channel] : "
                                     "failed to create opus decoder (code:%d)", this->opusErrorCode);

    if(this->handle == NULL || this->decoder == nullptr)
    {
        if(this->decoder != nullptr) opus_decoder_destroy(this->decoder);
        if(this->handle != NULL) BASS_StreamFree(this->handle);
        throw std::exception();
    }
}

Channel::~Channel() noexcept
{
    if(this->playing) this->channelstop = true;
    //if(this->playing && this->stopCallback != nullptr)
    //this->stopCallback(*this);

    opus_decoder_destroy(this->decoder);
    BASS_StreamFree(this->handle);
}

HSTREAM Channel::GetHandle() const noexcept
{
    return this->handle;
}

void Channel::SetSpeaker(const uint16_t speaker) noexcept
{
    this->speaker = speaker;
}

bool Channel::HasSpeaker() const noexcept
{
    return this->speaker != SV::kNonePlayer;
}

uint16_t Channel::GetSpeaker() const noexcept
{
    return this->speaker;
}

bool Channel::IsActive() const noexcept
{
    const auto bufferSize = BASS_ChannelGetData(this->handle, nullptr, BASS_DATA_AVAILABLE);
    return bufferSize != -1 && bufferSize != 0;
}

void Channel::Reset() noexcept
{
    BASS_ChannelPause(this->handle);
    BASS_ChannelSetPosition(this->handle, 0, BASS_POS_BYTE);
    opus_decoder_ctl(this->decoder, OPUS_RESET_STATE);

    //if(this->playing && this->stopCallback != nullptr)
    //this->stopCallback(*this);

    this->speaker = SV::kNonePlayer;
    this->expectedPacketNumber = 0;
    this->initialized = false;
    this->playing = false;
}

void Channel::Push(const uint32_t packetNumber, const uint8_t* const dataPtr, const uint32_t dataSize) noexcept
{
    if(!this->initialized || packetNumber == NULL)
    {
        FLog("[sv:dbg:channel:push] : init channel (speaker:%hu)", this->speaker);

        BASS_ChannelPause(this->handle);
        BASS_ChannelSetPosition(this->handle, 0, BASS_POS_BYTE);
        opus_decoder_ctl(this->decoder, OPUS_RESET_STATE);

        if(this->playing) this->channelstop = true;
        //if(this->playing && this->stopCallback != nullptr)
        //this->stopCallback(*this);

        this->initialized = true;
        this->playing = false;
    }
    else if(packetNumber < this->expectedPacketNumber)
    {
        FLog("[sv:dbg:channel:push] : late packet to channel (speaker:%hu) "
            "(pack:%u;expPack:%u)", this->speaker, packetNumber, this->expectedPacketNumber);

        return;
    }
    else if(packetNumber > this->expectedPacketNumber)
    {
        FLog("[sv:dbg:channel:push] : lost packet to channel (speaker:%hu) "
            "(pack:%u;expPack:%u)", this->speaker, packetNumber, this->expectedPacketNumber);

        if(const int length = opus_decode(this->decoder, dataPtr, dataSize, this->decBuffer.data(),
                                          SV::kFrameSizeInSamples, true); length == static_cast<int>(SV::kFrameSizeInSamples))
        {
            BASS_StreamPutData(this->handle, this->decBuffer.data(), SV::kFrameSizeInBytes);
        }
    }

    if(const int length = opus_decode(this->decoder, dataPtr, dataSize, this->decBuffer.data(),
                                      SV::kFrameSizeInSamples, false); length == static_cast<int>(SV::kFrameSizeInSamples))
    {
        BASS_StreamPutData(this->handle, this->decBuffer.data(), SV::kFrameSizeInBytes);
    }

    const auto channelStatus = BASS_ChannelIsActive(this->handle);
    const auto bufferSize = BASS_ChannelGetData(this->handle, nullptr, BASS_DATA_AVAILABLE);

    if((channelStatus == BASS_ACTIVE_PAUSED || channelStatus == BASS_ACTIVE_STOPPED) &&
       bufferSize != -1 && bufferSize >= SV::kChannelPreBufferFramesCount * SV::kFrameSizeInBytes)
    {
        //if (BlackList::IsPlayerBlocked(speaker))
           // return;


        FLog("[sv:dbg:channel:push] : playing channel (speaker:%hu)", this->speaker);

        BASS_ChannelPlay(this->handle, 0);

        if(!this->playing) this->channelplay = true;
        //if(!this->playing && this->playCallback != nullptr)
        //this->playCallback(*this);

        this->playing = true;
    }

    this->expectedPacketNumber = packetNumber + 1;
}

void Channel::SetPlayCallback(PlayCallback playCallback) noexcept
{
    this->playCallback = std::move(playCallback);
}

void Channel::SetStopCallback(StopCallback stopCallback) noexcept
{
    this->stopCallback = std::move(stopCallback);
}