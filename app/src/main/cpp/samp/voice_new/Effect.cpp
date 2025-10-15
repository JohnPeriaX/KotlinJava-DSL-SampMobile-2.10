#include "Effect.h"
#include "main.h"

Effect::Effect(const uint32_t type, const int priority,
               const void* const paramPtr, const uint32_t paramSize)
    : type(type), priority(priority), params(paramSize)
{
    std::memcpy(this->params.data(), paramPtr, paramSize);
}

Effect::~Effect() noexcept
{
    for (const auto& fxHandle : this->fxHandles)
    {
        BASS_ChannelRemoveFX(fxHandle.first, fxHandle.second);
    }
}

void Effect::Apply(const Channel& channel)
{
    if (const auto fxHandle = BASS_ChannelSetFX(channel.GetHandle(),
        this->type, this->priority); fxHandle != NULL)
    {
        if (BASS_FXSetParameters(fxHandle, this->params.data()) == FALSE)
        {
            LogVoice("[sv:err:effect:apply] : failed "
                "to set parameters (code:%d)", BASS_ErrorGetCode());
            BASS_ChannelRemoveFX(channel.GetHandle(), fxHandle);
        }
        else
        {
            this->fxHandles[channel.GetHandle()] = fxHandle;
        }
    }
    else
    {
        LogVoice("[sv:err:effect:apply] : failed to create "
            "effect (code:%d)", BASS_ErrorGetCode());
    }
}
