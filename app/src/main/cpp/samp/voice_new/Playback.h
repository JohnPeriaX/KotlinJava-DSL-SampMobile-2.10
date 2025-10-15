#pragma once

class Playback {
    Playback() = delete;
    ~Playback() = delete;
    Playback(const Playback&) = delete;
    Playback(Playback&&) = delete;
    Playback& operator=(const Playback&) = delete;
    Playback& operator=(Playback&&) = delete;

public:
    static bool Init() noexcept;
    static void Free() noexcept;

    static void Tick() noexcept;
    
    static bool GetSoundEnable() noexcept;
    static int GetSoundVolume() noexcept;
    static bool GetSoundBalancer() noexcept;
    static bool GetSoundFilter() noexcept;

    static void SetSoundEnable(bool soundEnable) noexcept;
    static void SetSoundVolume(int soundVolume) noexcept;
    static void SetSoundBalancer(bool soundBalancer) noexcept;
    static void SetSoundFilter(bool soundFilter) noexcept;

    static void SyncConfigs() noexcept;
    static void ResetConfigs() noexcept;

private:
    static bool BassInitHookFunc() noexcept;

private:
    static bool initStatus;
    static bool loadStatus;

    static HSTREAM deviceOutputChannel;
};
