#include "main.h"

#include "Render.h"

bool CVoiceRender::Init() noexcept
{
    if(CVoiceRender::initStatus)
        return false;

    LogVoice("[dbg:render:init] : module initializing...");

    CVoiceRender::deviceInitCallbacks.clear();
    CVoiceRender::renderCallbacks.clear();
    CVoiceRender::deviceFreeCallbacks.clear();

    LogVoice("[dbg:render:init] : module initialized");

    CVoiceRender::initStatus = true;

    return true;
}

void CVoiceRender::Free() noexcept
{
    if(!CVoiceRender::initStatus) return;

    LogVoice("[dbg:render:free] : module releasing...");

    for(const auto& deviceFreeCallback : CVoiceRender::deviceFreeCallbacks)
	{
		if(deviceFreeCallback != nullptr) 
			deviceFreeCallback();
	}

    CVoiceRender::deviceInitCallbacks.clear();
    CVoiceRender::renderCallbacks.clear();
    CVoiceRender::deviceFreeCallbacks.clear();

    LogVoice("[dbg:render:free] : module released");

    CVoiceRender::initStatus = false;
}

std::size_t CVoiceRender::AddDeviceInitCallback(DeviceInitCallback callback) noexcept
{
    if(!CVoiceRender::initStatus) return -1;

    for(std::size_t i { 0 }; i < CVoiceRender::deviceInitCallbacks.size(); ++i)
    {
        if(CVoiceRender::deviceInitCallbacks[i] == nullptr)
        {
            CVoiceRender::deviceInitCallbacks[i] = std::move(callback);
            return i;
        }
    }

    CVoiceRender::deviceInitCallbacks.emplace_back(std::move(callback));
    return CVoiceRender::deviceInitCallbacks.size() - 1;
}

std::size_t CVoiceRender::AddRenderCallback(RenderCallback callback) noexcept
{
    if(!CVoiceRender::initStatus) return -1;

    for(std::size_t i { 0 }; i < CVoiceRender::renderCallbacks.size(); ++i)
    {
        if(CVoiceRender::renderCallbacks[i] == nullptr)
        {
            CVoiceRender::renderCallbacks[i] = std::move(callback);
            return i;
        }
    }

    CVoiceRender::renderCallbacks.emplace_back(std::move(callback));
    return CVoiceRender::renderCallbacks.size() - 1;
}

std::size_t CVoiceRender::AddDeviceFreeCallback(DeviceFreeCallback callback) noexcept
{
    if(!CVoiceRender::initStatus) return -1;

    for(std::size_t i { 0 }; i < CVoiceRender::deviceFreeCallbacks.size(); ++i)
    {
        if(CVoiceRender::deviceFreeCallbacks[i] == nullptr)
        {
            CVoiceRender::deviceFreeCallbacks[i] = std::move(callback);
            return i;
        }
    }

    CVoiceRender::deviceFreeCallbacks.emplace_back(std::move(callback));
    return CVoiceRender::deviceFreeCallbacks.size() - 1;
}

void CVoiceRender::RemoveDeviceInitCallback(const std::size_t callback) noexcept
{
    if(!CVoiceRender::initStatus) return;

    if(callback >= CVoiceRender::deviceInitCallbacks.size())
        return;

    CVoiceRender::deviceInitCallbacks[callback] = nullptr;
}

void CVoiceRender::RemoveRenderCallback(const std::size_t callback) noexcept
{
    if(!CVoiceRender::initStatus) return;

    if(callback >= CVoiceRender::renderCallbacks.size())
        return;

    CVoiceRender::renderCallbacks[callback] = nullptr;
}

void CVoiceRender::RemoveDeviceFreeCallback(const std::size_t callback) noexcept
{
    if(!CVoiceRender::initStatus) return;

    if(callback >= CVoiceRender::deviceFreeCallbacks.size())
        return;

    CVoiceRender::deviceFreeCallbacks[callback] = nullptr;
}

bool CVoiceRender::initStatus {false };

std::vector<CVoiceRender::DeviceInitCallback> CVoiceRender::deviceInitCallbacks;
std::vector<CVoiceRender::RenderCallback> CVoiceRender::renderCallbacks;
std::vector<CVoiceRender::DeviceFreeCallback> CVoiceRender::deviceFreeCallbacks;
