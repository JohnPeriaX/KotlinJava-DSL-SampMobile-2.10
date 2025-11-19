#pragma once

#include "../SPSCQueue.h"

class KeyFilter {
    KeyFilter() = delete;
    ~KeyFilter() = delete;
    KeyFilter(const KeyFilter&) = delete;
    KeyFilter(KeyFilter&&) = delete;
    KeyFilter& operator=(const KeyFilter&) = delete;
    KeyFilter& operator=(KeyFilter&&) = delete;

public:
    static bool AddKey(uint16_t keyId) noexcept;
    static bool RemoveKey(uint16_t keyId) noexcept;

    static void RemoveAllKeys() noexcept;

    static bool IsKeyAvailable(uint16_t keyId) noexcept;

private:
    static std::array<bool, 256> statusKeys;
};
