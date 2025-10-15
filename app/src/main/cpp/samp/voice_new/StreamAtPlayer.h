#pragma once

#include "../game/common.h"

#include "LocalStream.h"
#include "Channel.h"

class StreamAtPlayer : public LocalStream {
    StreamAtPlayer() = delete;
    StreamAtPlayer(const StreamAtPlayer&) = delete;
    StreamAtPlayer(StreamAtPlayer&&) = delete;
    StreamAtPlayer& operator=(const StreamAtPlayer&) = delete;
    StreamAtPlayer& operator=(StreamAtPlayer&&) = delete;

public:
    explicit StreamAtPlayer(uint32_t color, std::string name,
                            float distance, PLAYERID playerId) noexcept;

    ~StreamAtPlayer() noexcept = default;

public:
    void Tick() noexcept override;

private:
    void OnChannelCreate(const Channel& channel) noexcept override;

private:
    const PLAYERID playerId;
};

using StreamAtPlayerPtr = std::unique_ptr<StreamAtPlayer>;
#define MakeStreamAtPlayer std::make_unique<StreamAtPlayer>
