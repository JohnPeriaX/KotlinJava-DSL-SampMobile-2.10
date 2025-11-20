#pragma once

class CVoiceRender {
    CVoiceRender() = delete;
    ~CVoiceRender() = delete;
    CVoiceRender(const CVoiceRender&) = delete;
    CVoiceRender(CVoiceRender&&) = delete;
    CVoiceRender& operator=(const CVoiceRender&) = delete;
    CVoiceRender& operator=(CVoiceRender&&) = delete;

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
