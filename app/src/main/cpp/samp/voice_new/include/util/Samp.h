#pragma once

#include <functional>
#include <vector>

class Samp {
    Samp() = delete;
    ~Samp() = delete;
    Samp(const Samp&) = delete;
    Samp(Samp&&) = delete;
    Samp& operator=(const Samp&) = delete;
    Samp& operator=(Samp&&) = delete;

private:
    using LoadCallback = std::function<void()>;
    using ExitCallback = std::function<void()>;

public:
    static bool Init() noexcept;
    static bool IsInited() noexcept;
    static bool IsLoaded() noexcept;
    static void Free() noexcept;

    static size_t AddLoadCallback(LoadCallback callback) noexcept;
    static size_t AddExitCallback(ExitCallback callback) noexcept;

    static void RemoveLoadCallback(size_t callback) noexcept;
    static void RemoveExitCallback(size_t callback) noexcept;

public:
    static bool initStatus;
    static bool loadStatus;

    static std::vector<LoadCallback> loadCallbacks;
    static std::vector<ExitCallback> exitCallbacks;
};
