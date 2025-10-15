#include "../../../main.h"

#include "Render.h"

bool Render::Init() noexcept
{
    if(Render::initStatus) 
        return false;

    LogVoice("[dbg:render:init] : module initializing...");

    Render::deviceInitCallbacks.clear();
    Render::renderCallbacks.clear();
    Render::deviceFreeCallbacks.clear();

    LogVoice("[dbg:render:init] : module initialized");

    Render::initStatus = true;

    return true;
}

void Render::Free() noexcept
{
    if(!Render::initStatus) return;

    LogVoice("[dbg:render:free] : module releasing...");

    for(const auto& deviceFreeCallback : Render::deviceFreeCallbacks)
	{
		if(deviceFreeCallback != nullptr) 
			deviceFreeCallback();
	}

    Render::deviceInitCallbacks.clear();
    Render::renderCallbacks.clear();
    Render::deviceFreeCallbacks.clear();

    LogVoice("[dbg:render:free] : module released");

    Render::initStatus = false;
}

std::size_t Render::AddDeviceInitCallback(DeviceInitCallback callback) noexcept
{
    if(!Render::initStatus) return -1;

    for(std::size_t i { 0 }; i < Render::deviceInitCallbacks.size(); ++i)
    {
        if(Render::deviceInitCallbacks[i] == nullptr)
        {
            Render::deviceInitCallbacks[i] = std::move(callback);
            return i;
        }
    }

    Render::deviceInitCallbacks.emplace_back(std::move(callback));
    return Render::deviceInitCallbacks.size() - 1;
}

std::size_t Render::AddRenderCallback(RenderCallback callback) noexcept
{
    if(!Render::initStatus) return -1;

    for(std::size_t i { 0 }; i < Render::renderCallbacks.size(); ++i)
    {
        if(Render::renderCallbacks[i] == nullptr)
        {
            Render::renderCallbacks[i] = std::move(callback);
            return i;
        }
    }

    Render::renderCallbacks.emplace_back(std::move(callback));
    return Render::renderCallbacks.size() - 1;
}

std::size_t Render::AddDeviceFreeCallback(DeviceFreeCallback callback) noexcept
{
    if(!Render::initStatus) return -1;

    for(std::size_t i { 0 }; i < Render::deviceFreeCallbacks.size(); ++i)
    {
        if(Render::deviceFreeCallbacks[i] == nullptr)
        {
            Render::deviceFreeCallbacks[i] = std::move(callback);
            return i;
        }
    }

    Render::deviceFreeCallbacks.emplace_back(std::move(callback));
    return Render::deviceFreeCallbacks.size() - 1;
}

void Render::RemoveDeviceInitCallback(const std::size_t callback) noexcept
{
    if(!Render::initStatus) return;

    if(callback >= Render::deviceInitCallbacks.size())
        return;

    Render::deviceInitCallbacks[callback] = nullptr;
}

void Render::RemoveRenderCallback(const std::size_t callback) noexcept
{
    if(!Render::initStatus) return;

    if(callback >= Render::renderCallbacks.size())
        return;

    Render::renderCallbacks[callback] = nullptr;
}

void Render::RemoveDeviceFreeCallback(const std::size_t callback) noexcept
{
    if(!Render::initStatus) return;

    if(callback >= Render::deviceFreeCallbacks.size())
        return;

    Render::deviceFreeCallbacks[callback] = nullptr;
}

bool Render::initStatus { false };

std::vector<Render::DeviceInitCallback> Render::deviceInitCallbacks;
std::vector<Render::RenderCallback> Render::renderCallbacks;
std::vector<Render::DeviceFreeCallback> Render::deviceFreeCallbacks;
