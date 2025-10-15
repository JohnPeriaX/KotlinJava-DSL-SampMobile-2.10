#pragma once

#include <string>

class PluginConfig {
    PluginConfig() = delete;
    ~PluginConfig() = delete;
    PluginConfig(const PluginConfig&) = delete;
    PluginConfig(PluginConfig&&) = delete;
    PluginConfig& operator=(const PluginConfig&) = delete;
    PluginConfig& operator=(PluginConfig&&) = delete;

public:
    static constexpr bool     kDefValSoundEnable        = true;
    static constexpr int      kDefValSoundVolume        = 100;
    static constexpr bool     kDefValSoundBalancer      = false;
    static constexpr bool     kDefValSoundFilter        = false;
    static constexpr bool     kDefValMicroEnable        = true;
    static constexpr int      kDefValMicroVolume        = 75;
    static constexpr float    kDefValSpeakerIconSize    = 46.0f;

public:
    static bool IsPlaybackLoaded() noexcept;
    static bool IsSpeakerLoaded() noexcept;
    static bool IsRecordLoaded() noexcept;
    static bool IsMicroLoaded() noexcept;

    static void SetPlaybackLoaded(bool status) noexcept;
    static void SetSpeakerLoaded(bool status) noexcept;
    static void SetRecordLoaded(bool status) noexcept;
    static void SetMicroLoaded(bool status) noexcept;

    static bool GetSoundEnable() noexcept;
    static int GetSoundVolume() noexcept;
    static bool GetSoundBalancer() noexcept;
    static bool GetSoundFilter() noexcept;
    static bool GetMicroEnable() noexcept;
    static int GetMicroVolume() noexcept;
    static const std::string& GetDeviceName() noexcept;

    static void SetSoundEnable(bool soundEnable) noexcept;
    static void SetSoundVolume(int soundVolume) noexcept;
    static void SetSoundBalancer(bool soundBalancer) noexcept;
    static void SetSoundFilter(bool soundFilter) noexcept;
    static void SetMicroEnable(bool microEnable) noexcept;
    static void SetMicroVolume(int microVolume) noexcept;
    static void SetDeviceName(std::string deviceName) noexcept;

private:
    static bool playbackLoaded;
    static bool speakerLoaded;
    static bool recordLoaded;
    static bool microLoaded;

    static bool soundEnable;
    static int soundVolume;
    static bool soundBalancer;
    static bool soundFilter;

    static bool microEnable;
    static int microVolume;
    static std::string deviceName;
};
