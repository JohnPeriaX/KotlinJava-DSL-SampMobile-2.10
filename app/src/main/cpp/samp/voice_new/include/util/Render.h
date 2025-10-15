#pragma once

#include <functional>

class Render {
    Render() = delete;
    ~Render() = delete;
    Render(const Render&) = delete;
    Render(Render&&) = delete;
    Render& operator=(const Render&) = delete;
    Render& operator=(Render&&) = delete;

private:
    using DeviceInitCallback = std::function<void()>;
    using RenderCallback = std::function<void()>;
    using DeviceFreeCallback = std::function<void()>;

public:
    static bool Init() noexcept;
    static void Free() noexcept;

public:
    static std::size_t AddDeviceInitCallback(DeviceInitCallback callback) noexcept;
    static std::size_t AddRenderCallback(RenderCallback callback) noexcept;
    static std::size_t AddDeviceFreeCallback(DeviceFreeCallback callback) noexcept;

    static void RemoveDeviceInitCallback(std::size_t callback) noexcept;
    static void RemoveRenderCallback(std::size_t callback) noexcept;
    static void RemoveDeviceFreeCallback(std::size_t callback) noexcept;

public:
    static std::vector<DeviceInitCallback> deviceInitCallbacks;
    static std::vector<RenderCallback> renderCallbacks;
    static std::vector<DeviceFreeCallback> deviceFreeCallbacks;

private:
    static bool initStatus;
};
