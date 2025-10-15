#pragma once

#include "../game/common.h"
#include "../net/netgame.h"

#include "LocalStream.h"
#include "Channel.h"

class StreamAtVehicle : public LocalStream {
    StreamAtVehicle() = delete;
    StreamAtVehicle(const StreamAtVehicle&) = delete;
    StreamAtVehicle(StreamAtVehicle&&) = delete;
    StreamAtVehicle& operator=(const StreamAtVehicle&) = delete;
    StreamAtVehicle& operator=(StreamAtVehicle&&) = delete;

public:
    explicit StreamAtVehicle(uint16_t color, std::string name,
                             float distance, VEHICLEID vehicleId) noexcept;

    ~StreamAtVehicle() noexcept = default;

public:
    void Tick() noexcept override;

private:
    void OnChannelCreate(const Channel& channel) noexcept override;

private:
    const VEHICLEID vehicleId;
};

using StreamAtVehiclePtr = std::unique_ptr<StreamAtVehicle>;
#define MakeStreamAtVehicle std::make_unique<StreamAtVehicle>
