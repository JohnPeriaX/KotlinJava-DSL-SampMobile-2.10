#pragma once

#include "Stream.h"

class GlobalStream : public Stream {
    GlobalStream() = delete;
    GlobalStream(const GlobalStream&) = delete;
    GlobalStream(GlobalStream&&) = delete;
    GlobalStream& operator=(const GlobalStream&) = delete;
    GlobalStream& operator=(GlobalStream&&) = delete;

public:
    explicit GlobalStream(uint32_t color, std::string name) noexcept;

    ~GlobalStream() noexcept = default;
};

using GlobalStreamPtr = std::unique_ptr<GlobalStream>;
#define MakeGlobalStream std::make_unique<GlobalStream>
