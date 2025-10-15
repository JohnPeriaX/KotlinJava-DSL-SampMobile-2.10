#pragma once

#include "LocalStream.h"
#include "Channel.h"

class StreamAtObject : public LocalStream {
    StreamAtObject() = delete;
    StreamAtObject(const StreamAtObject&) = delete;
    StreamAtObject(StreamAtObject&&) = delete;
    StreamAtObject& operator=(const StreamAtObject&) = delete;
    StreamAtObject& operator=(StreamAtObject&&) = delete;

public:
    explicit StreamAtObject(uint32_t color, std::string name,
                            float distance, WORD objectId) noexcept;

    ~StreamAtObject() noexcept = default;

public:
    void Tick() noexcept override;

private:
    void OnChannelCreate(const Channel& channel) noexcept override;

private:
    const uint16_t objectId;
};

using StreamAtObjectPtr = std::unique_ptr<StreamAtObject>;
#define MakeStreamAtObject std::make_unique<StreamAtObject>
