#pragma once

#include "Parameter.h"
#include "Channel.h"

class SetController : public Parameter {
    SetController() = delete;
    SetController(const SetController&) = delete;
    SetController(SetController&&) = delete;
    SetController& operator=(const SetController&) = delete;
    SetController& operator=(SetController&&) = delete;

public:
    explicit SetController(uint32_t parameter, float value) noexcept;

    ~SetController() noexcept = default;

public:
    void Apply(const Channel& channel) const noexcept override;

private:
    const float value;
};

using SetControllerPtr = std::unique_ptr<SetController>;
#define MakeSetController std::make_unique<SetController>
