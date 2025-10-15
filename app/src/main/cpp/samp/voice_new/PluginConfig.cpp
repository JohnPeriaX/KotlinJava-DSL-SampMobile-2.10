#include "PluginConfig.h"

bool PluginConfig::IsPlaybackLoaded() noexcept
{
    return PluginConfig::playbackLoaded;
}

bool PluginConfig::IsSpeakerLoaded() noexcept
{
    return PluginConfig::speakerLoaded;
}

bool PluginConfig::IsRecordLoaded() noexcept
{
    return PluginConfig::recordLoaded;
}

bool PluginConfig::IsMicroLoaded() noexcept
{
    return PluginConfig::microLoaded;
}

void PluginConfig::SetPlaybackLoaded(const bool status) noexcept
{
    PluginConfig::playbackLoaded = status;
}

void PluginConfig::SetSpeakerLoaded(const bool status) noexcept
{
    PluginConfig::speakerLoaded = status;
}

void PluginConfig::SetRecordLoaded(const bool status) noexcept
{
    PluginConfig::recordLoaded = status;
}

void PluginConfig::SetMicroLoaded(const bool status) noexcept
{
    PluginConfig::microLoaded = status;
}

bool PluginConfig::GetSoundEnable() noexcept
{
    return PluginConfig::soundEnable;
}

int PluginConfig::GetSoundVolume() noexcept
{
    return PluginConfig::soundVolume;
}

bool PluginConfig::GetSoundBalancer() noexcept
{
    return PluginConfig::soundBalancer;
}

bool PluginConfig::GetSoundFilter() noexcept
{
    return PluginConfig::soundFilter;
}

bool PluginConfig::GetMicroEnable() noexcept
{
    return PluginConfig::microEnable;
}

int PluginConfig::GetMicroVolume() noexcept
{
    return PluginConfig::microVolume;
}

const std::string& PluginConfig::GetDeviceName() noexcept
{
    return PluginConfig::deviceName;
}

void PluginConfig::SetSoundEnable(const bool soundEnable) noexcept
{
    PluginConfig::soundEnable = soundEnable;
}

void PluginConfig::SetSoundVolume(const int soundVolume) noexcept
{
    PluginConfig::soundVolume = soundVolume;
}

void PluginConfig::SetSoundBalancer(const bool soundBalancer) noexcept
{
    PluginConfig::soundBalancer = soundBalancer;
}

void PluginConfig::SetSoundFilter(const bool soundFilter) noexcept
{
    PluginConfig::soundFilter = soundFilter;
}

void PluginConfig::SetMicroEnable(const bool microEnable) noexcept
{
    PluginConfig::microEnable = microEnable;
}

void PluginConfig::SetMicroVolume(const int microVolume) noexcept
{
    PluginConfig::microVolume = microVolume;
}

void PluginConfig::SetDeviceName(std::string deviceName) noexcept
{
    PluginConfig::deviceName = std::move(deviceName);
}

bool PluginConfig::playbackLoaded { false };
bool PluginConfig::speakerLoaded { false };
bool PluginConfig::recordLoaded { false };
bool PluginConfig::microLoaded { false };

bool PluginConfig::soundEnable { kDefValSoundEnable };
int PluginConfig::soundVolume { kDefValSoundVolume };
bool PluginConfig::soundBalancer { kDefValSoundBalancer };
bool PluginConfig::soundFilter { kDefValSoundFilter };

bool PluginConfig::microEnable { kDefValMicroEnable };
int PluginConfig::microVolume { kDefValMicroVolume };
std::string PluginConfig::deviceName;

