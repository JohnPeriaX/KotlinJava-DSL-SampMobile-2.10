#pragma once

#include "ShadowCamera.h"
#include "Entity/CPhysical.h"

class CRealTimeShadow;
class CRealTimeShadowManager {
public:
    static constexpr int32_t NUM_REALTIME_SHADOWS = 40;

public:
    bool             m_bInitialised{};
    CRealTimeShadow* m_apShadows[NUM_REALTIME_SHADOWS]{};
    bool             m_bNeedsReinit{}; // AKA `m_bNotReInitialized`
    CShadowCamera    m_BlurCamera{};
    CShadowCamera    m_GradientCamera{};

public:
    static void InjectHooks();
//
//    void Init();
//    void Exit();
//    void ReInit();
//    void Update();
//
    CRealTimeShadow* GetRealTimeShadow(CPhysical* physical);
    void             DoShadowThisFrame(CPhysical* physical);
    void             ReturnRealTimeShadow(CRealTimeShadow* pShadow);
};
VALIDATE_SIZE(CRealTimeShadowManager, (VER_x32 ? 0xB8 : 0x170));

extern CRealTimeShadowManager& g_realTimeShadowMan;
