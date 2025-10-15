#pragma once

#include "Stream.h"
#include "StreamInfo.h"
#include "Channel.h"

class LocalStream : public Stream {
    LocalStream() = delete;
    LocalStream(const LocalStream&) = delete;
    LocalStream(LocalStream&&) = delete;
    LocalStream& operator=(const LocalStream&) = delete;
    LocalStream& operator=(LocalStream&&) = delete;

protected:
    explicit LocalStream(StreamType type, uint32_t color,
                            std::string name, float distance) noexcept;

public:
    virtual ~LocalStream() noexcept = default;

public:
    void SetDistance(float distance) noexcept;

protected:
    void OnChannelCreate(const Channel& channel) noexcept override;

private:
    float distance;
};

using LocalStreamPtr = std::unique_ptr<LocalStream>;
