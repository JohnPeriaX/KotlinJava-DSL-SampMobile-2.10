#pragma once

#include "StreamInfo.h"
#include "VoicePacket.h"
#include "Parameter.h"
#include "Channel.h"
#include "Effect.h"

class Stream {
    Stream() = delete;
    Stream(const Stream&) = delete;
    Stream(Stream&&) = delete;
    Stream& operator=(const Stream&) = delete;
    Stream& operator=(Stream&&) = delete;

private:
    using PlayCallback = std::function<void(const Stream&, uint16_t)>;
    using StopCallback = std::function<void(const Stream&, uint16_t)>;

protected:
    explicit Stream(uint32_t streamFlags, StreamType type, uint32_t color, std::string name) noexcept;

public:
    virtual ~Stream() noexcept = default;

public:
    const StreamInfo& GetInfo() const noexcept;

    virtual void Tick() noexcept;
    void Push(const VoicePacket& packet);
    void Reset() noexcept;
    void SetParameter(uint8_t parameter, float value);
    void SlideParameter(uint8_t parameter, float startValue, float endValue, uint32_t time);
    void EffectCreate(uint32_t effect, uint32_t number, int priority, const void* paramPtr, uint32_t paramSize);
    void EffectDelete(uint32_t effect);

    std::size_t AddPlayCallback(PlayCallback playCallback);
    std::size_t AddStopCallback(StopCallback stopCallback);
    void RemovePlayCallback(std::size_t callback) noexcept;
    void RemoveStopCallback(std::size_t callback) noexcept;

protected:
    virtual void OnChannelCreate(const Channel& channel);

private:
    void OnChannelPlay(const Channel& channel) noexcept;
    void OnChannelStop(const Channel& channel) noexcept;

protected:
    const std::vector<ChannelPtr>& GetChannels() const noexcept;

private:
    const uint32_t streamFlags;
    const StreamInfo streamInfo;

    std::vector<ChannelPtr> channels;

    std::vector<PlayCallback> playCallbacks;
    std::vector<StopCallback> stopCallbacks;

    std::map<uint8_t, ParameterPtr> parameters;
    std::map<uint32_t, EffectPtr> effects;
};

using StreamPtr = std::unique_ptr<Stream>;
