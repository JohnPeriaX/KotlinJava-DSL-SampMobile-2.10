#pragma once

#include "../main.h"

enum class StreamType
{
    None,
    GlobalStream,
    LocalStreamAtPoint,
    LocalStreamAtVehicle,
    LocalStreamAtPlayer,
    LocalStreamAtObject
};

struct StreamInfo {
    StreamInfo() {}

public:
    StreamInfo(StreamType type, uint32_t color, std::string name) noexcept;
    ~StreamInfo() noexcept = default;

public:
    StreamType GetType() const noexcept;
    uint32_t GetColor() const noexcept;
    const std::string& GetName() const noexcept;

private:
    StreamType type { StreamType::None };
    uint32_t color { -1u };
    std::string name;
};
