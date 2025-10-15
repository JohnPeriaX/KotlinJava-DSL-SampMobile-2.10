//
// Created by roman on 11/24/2024.
//

#pragma once

#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)

#include <stdint.h>

enum MoveLimbResult { CANT_REACH_TARGET, HAVENT_REACHED_TARGET, REACHED_TARGET };

struct LimbOrientation {
public:
    float m_fYaw{0.0f};
    float m_fPitch{0.0f};

    LimbOrientation() = default;
    LimbOrientation(float yaw, float pitch) : m_fYaw(yaw), m_fPitch(pitch) { }
};
VALIDATE_SIZE(LimbOrientation, 0x8);

struct LimbMovementInfo {
    float maxYaw, minYaw;
    float yawD;
    float maxPitch, minPitch;
    float pitchD;
};
VALIDATE_SIZE(LimbMovementInfo, 0x18);

struct CPedGTA;
class CPedIK {
public:
    CPedGTA* m_pPed{nullptr};
    LimbOrientation m_TorsoOrient{};
    float m_fSlopePitch{0.0f};
    float m_fSlopePitchLimitMult{0.0f};
    float m_fSlopeRoll{0.0f};
    float m_fBodyRoll{0.0f};

    union {
        uint32_t m_nFlags{0};
        struct {
            bool bGunReachedTarget : 1;
            bool bTorsoUsed : 1;
            bool bUseArm : 1;
            bool bSlopePitch : 1;
            bool bUnk : 1;
        };
    };
};

VALIDATE_SIZE(CPedIK, (VER_x32 ? 0x20 : 0x28));