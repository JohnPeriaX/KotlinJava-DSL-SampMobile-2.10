#pragma once

#include "../vendor/bass/bass.h"

#include "Channel.h"

#include <vector>
#include <map>
#include <cstring>

class Effect {
    Effect() = delete;
    Effect(const Effect&) = delete;
    Effect(Effect&&) = delete;
    Effect& operator=(const Effect&) = delete;
    Effect& operator=(Effect&&) = delete;

public:
    explicit Effect(uint32_t type, int priority,  const void* paramPtr, uint32_t paramSize);

    ~Effect() noexcept;

public:
    void Apply(const Channel& channel);

private:
    const uint32_t type;
    const int priority;
    std::vector<uint8_t> params;

    std::map<HSTREAM, HFX> fxHandles;

};

using EffectPtr = std::unique_ptr<Effect>;
#define MakeEffect std::make_unique<Effect>
