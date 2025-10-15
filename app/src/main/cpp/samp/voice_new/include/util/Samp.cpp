
#include "Samp.h"

void LogVoice(const char* fmt, ...);

bool Samp::Init() noexcept
{
    if(Samp::initStatus)
        return false;

    LogVoice("[dbg:samp:init] : module initializing...");

    Samp::loadCallbacks.clear();
    Samp::exitCallbacks.clear();

    Samp::loadStatus = false;

    LogVoice("[dbg:samp:init] : module initialized");

    Samp::initStatus = true;

    return true;
}

bool Samp::IsInited() noexcept
{
    return Samp::initStatus;
}

bool Samp::IsLoaded() noexcept
{
    return Samp::loadStatus;
}

void Samp::Free() noexcept
{
    if(!Samp::initStatus)
        return;

    LogVoice("[dbg:samp:free] : module releasing...");

    if(Samp::loadStatus)
    {
        for(const auto& exitCallback : Samp::exitCallbacks)
        {
            if(exitCallback != nullptr)
                exitCallback();
        }
    }

    Samp::loadStatus = false;

    Samp::loadCallbacks.clear();
    Samp::exitCallbacks.clear();

    LogVoice("[dbg:samp:free] : module released");

    Samp::initStatus = false;
}

std::size_t Samp::AddLoadCallback(LoadCallback callback) noexcept
{
    if(!Samp::initStatus) return -1;

    for(std::size_t i { 0 }; i < Samp::loadCallbacks.size(); ++i)
    {
        if(Samp::loadCallbacks[i] == nullptr)
        {
            Samp::loadCallbacks[i] = std::move(callback);
            return i;
        }
    }

    Samp::loadCallbacks.emplace_back(std::move(callback));
    return Samp::loadCallbacks.size() - 1;
}

std::size_t Samp::AddExitCallback(ExitCallback callback) noexcept
{
    if(!Samp::initStatus) return -1;

    for(std::size_t i { 0 }; i < Samp::exitCallbacks.size(); ++i)
    {
        if(Samp::exitCallbacks[i] == nullptr)
        {
            Samp::exitCallbacks[i] = std::move(callback);
            return i;
        }
    }

    Samp::exitCallbacks.emplace_back(std::move(callback));
    return Samp::exitCallbacks.size() - 1;
}

void Samp::RemoveLoadCallback(const std::size_t callback) noexcept
{
    if(!Samp::initStatus) return;

    if(callback >= Samp::loadCallbacks.size())
        return;

    Samp::loadCallbacks[callback] = nullptr;
}

void Samp::RemoveExitCallback(const std::size_t callback) noexcept
{
    if(!Samp::initStatus) return;

    if(callback >= Samp::exitCallbacks.size())
        return;

    Samp::exitCallbacks[callback] = nullptr;
}

bool Samp::initStatus { false };
bool Samp::loadStatus { false };

std::vector<Samp::LoadCallback> Samp::loadCallbacks;
std::vector<Samp::ExitCallback> Samp::exitCallbacks;