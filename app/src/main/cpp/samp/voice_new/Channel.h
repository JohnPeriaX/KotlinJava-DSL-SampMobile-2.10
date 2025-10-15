#pragma once

#include "../vendor/bass/bass.h"
#include <opus.h>
#include <vector>
#include <array>

#include "Header.h"

class Channel {
    Channel() = delete;
    Channel(const Channel&) = delete;
    Channel(Channel&&) = delete;
    Channel& operator=(const Channel&) = delete;
    Channel& operator=(Channel&&) = delete;

private:
    using PlayCallback = std::function<void(const Channel&)>;
    using StopCallback = std::function<void(const Channel&)>;

public:
    explicit Channel(uint32_t channelFlags);

    ~Channel() noexcept;

public:
    HSTREAM GetHandle() const noexcept;
    void SetSpeaker(uint16_t speaker) noexcept;
    bool HasSpeaker() const noexcept;
    uint16_t GetSpeaker() const noexcept;

    bool IsActive() const noexcept;
    void Reset() noexcept;
    void Push(uint32_t packetNumber, const uint8_t* dataPtr, uint32_t dataSize) noexcept;

    void SetPlayCallback(PlayCallback playCallback) noexcept;
    void SetStopCallback(StopCallback stopCallback) noexcept;

public:
    bool playing { false };
    bool channelplay { false };
    bool channelstop { false };

private:
    const HSTREAM handle;
    uint16_t speaker { SV::kNonePlayer };

    PlayCallback playCallback;
    StopCallback stopCallback;

    OpusDecoder* const decoder;
    std::array<opus_int16, SV::kFrameSizeInSamples> decBuffer;

    uint32_t expectedPacketNumber { 0 };
    bool initialized { false };

    int opusErrorCode { -1 };
};

using ChannelPtr = std::unique_ptr<Channel>;
#define MakeChannel std::make_unique<Channel>
