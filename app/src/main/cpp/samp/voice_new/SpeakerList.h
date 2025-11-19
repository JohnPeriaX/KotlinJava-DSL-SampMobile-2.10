#pragma once
#include "../gui/gui.h"
#include "../net/netgame.h"
#include "../game/Core/Vector.h"

#include "Stream.h"

#include <unordered_map>
#include <array>

class SpeakerList {
    friend class CGUI;

    SpeakerList() = delete;
    ~SpeakerList() = delete;
    SpeakerList(const SpeakerList&) = delete;
    SpeakerList(SpeakerList&&) = delete;
    SpeakerList& operator=(const SpeakerList&) = delete;
    SpeakerList& operator=(SpeakerList&&) = delete;

private:
    static constexpr float kBaseIconSize = 25.f;

public:
    static bool Init() noexcept;
    static void Free() noexcept;

    static void Show() noexcept;
    static bool IsShowed() noexcept;
    static void Hide() noexcept;

    static void Render();
    static void Draw(CVector* vec, float fDist);

    static float GetSpeakerIconScale() noexcept;

    static void SetSpeakerIconScale(float speakerIconScale) noexcept;

    static void SyncConfigs() noexcept;
    static void ResetConfigs() noexcept;

public:
    static void OnSpeakerPlay(const Stream& stream, uint16_t speaker) noexcept;
    static void OnSpeakerStop(const Stream& stream, uint16_t speaker) noexcept;

private:
    static bool initStatus;
    static bool showStatus;

    static RwTexture* tSpeakerIcon;

    // Local state for speaker icon scale (clamped via SetSpeakerIconScale)
    static float speakerIconScale;

    static std::array<std::unordered_map<Stream*, StreamInfo>, MAX_PLAYERS> playerStreams;
    static std::array<const Stream*, MAX_PLAYERS> playerStream;
};
