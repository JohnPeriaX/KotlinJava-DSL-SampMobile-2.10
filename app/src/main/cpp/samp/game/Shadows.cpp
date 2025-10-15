//
// Created by plaka on 18.10.2023.
//

#include "Shadows.h"
#include "Camera.h"
#include "TxdStore.h"
#include "Core/Rect.h"
#include "game/Pipelines/CustomBuilding/CustomBuildingDNPipeline.h"
#include "patch.h"
#include "Timer.h"

void CShadows::RenderStaticShadows() {

}

void CShadows::AffectColourWithLighting(
        eShadowType shadowType,
        uint8 dayNightIntensity, // packed 2x4 bits for day/night
        uint8 r, uint8 g, uint8 b,
        uint8& outR, uint8& outG, uint8& outB
) {

}

void CShadows::CalcPedShadowValues(CVector sunPosn, float& displacementX, float& displacementY, float& frontX, float& frontY, float& sideX, float& sideY) {
    const auto sunDist = sunPosn.Magnitude2D();
    const auto recip = 1.0f / sunDist;
    const auto mult = (sunDist + 1.0f) * recip;

    displacementX = -sunPosn.x * mult / 2.0f;
    displacementY = -sunPosn.y * mult / 2.0f;

    frontX = -sunPosn.y * recip / 2.0f;
    frontY = +sunPosn.x * recip / 2.0f;

    sideX = -sunPosn.x / 2.0f;
    sideY = -sunPosn.y / 2.0f;
}

void CShadows::StoreCarLightShadow(CVehicleGTA* vehicle, int32 id, RwTexture* texture, CVector* posn, float frontX, float frontY, float sideX, float sideY, uint8 red, uint8 green, uint8 blue, float maxViewAngleCosine) {

}

void CShadows::StoreShadowToBeRendered(uint8 type, RwTexture* texture, const CVector& posn, float topX, float topY, float rightX, float rightY, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistance, bool drawOnWater, float scale, CRealTimeShadow* realTimeShadow, bool drawOnBuildings) {
    if (ShadowsStoredToBeRendered >= asShadowsStored.size())
        return;

    auto& shadow = asShadowsStored[ShadowsStoredToBeRendered];

    shadow.m_nType      = (eShadowType)type;
    shadow.m_pTexture   = texture;
    shadow.m_vecPosn    = posn;
    shadow.m_Front.x    = topX;
    shadow.m_Front.y    = topY;
    shadow.m_Side.x     = rightX;
    shadow.m_Side.y     = rightY;
    shadow.m_nIntensity = intensity;
    shadow.m_nRed       = red;
    shadow.m_nGreen     = green;
    shadow.m_nBlue      = blue;
    shadow.m_fZDistance = zDistance;
    shadow.m_bDrawOnWater     = drawOnWater;
    shadow.m_bDrawOnBuildings = drawOnBuildings;
    shadow.m_fScale     = scale;
    shadow.m_pRTShadow  = realTimeShadow;

    ShadowsStoredToBeRendered++;
}

void CShadows::Init() {
    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(CTxdStore::FindTxdSlot("particle"));

    gpShadowCarTex              = RwTextureRead("shad_car",     nullptr);
    gpShadowPedTex              = RwTextureRead("shad_ped",     nullptr);
    gpShadowHeliTex             = RwTextureRead("shad_heli",    nullptr);
    gpShadowBikeTex             = RwTextureRead("shad_bike",    nullptr);
    gpShadowBaronTex            = RwTextureRead("shad_rcbaron", nullptr);
    gpShadowExplosionTex        = RwTextureRead("shad_exp",     nullptr);
    gpShadowHeadLightsTex       = RwTextureRead("headlight",    nullptr);
    gpShadowHeadLightsTexLong   = RwTextureRead("headlight_l",  nullptr);
    gpShadowHeadLightsTex2      = RwTextureRead("headlight1",   nullptr);
    gpBloodPoolTex              = RwTextureRead("bloodpool_64", nullptr);
    gpHandManTex                = RwTextureRead("handman",      nullptr);
    gpCrackedGlassTex           = RwTextureRead("wincrack_32",  nullptr);
    gpPostShadowTex             = RwTextureRead("lamp_shad_64", nullptr);

    CTxdStore::PopCurrentTxd();

    g_ShadowVertices = { 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 5, 4, 0, 6, 5, 0, 7, 6, 0, 8, 7, 0, 9, 8 };

    for (auto& shadow : aStaticShadows) {
        shadow.Init();
    }

    pEmptyBunchList = aPolyBunches.data();

    for (auto i = 0u; i < aPolyBunches.size() - 1; i++) {
        aPolyBunches[i].m_pNext = &aPolyBunches[i + 1];
    }
    aPolyBunches.back().m_pNext = nullptr;

    for (auto& shadow : aPermanentShadows) {
        shadow.Init();
    }
}

void CShadows::StoreShadowToBeRendered(eShadowType type, RwTexture* tex, const CVector& posn, CVector2D top, CVector2D right, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistance, bool drawOnWater, float scale, CRealTimeShadow* realTimeShadow, bool drawOnBuildings) {
    StoreShadowToBeRendered(
            type,
            tex,
            posn,
            top.x, top.y,
            right.x, right.y,
            intensity,
            red, green, blue,
            zDistance,
            drawOnWater,
            scale,
            realTimeShadow,
            drawOnBuildings
    );
}

void CShadows::StoreShadowToBeRendered(uint8 type, const CVector& posn, float frontX, float frontY, float sideX, float sideY, int16 intensity, uint8 red, uint8 green, uint8 blue) {
    const auto Store = [=](auto mtype, auto texture) {
        StoreShadowToBeRendered(mtype, texture, posn, frontX, frontY, sideX, sideY, intensity, red, green, blue, 15.0f, 0, 1.0f, nullptr, 0);
    };

    switch (type) {
        case SHADOW_DEFAULT:
            Store(SHADOW_TEX_CAR, gpShadowCarTex);
            break;
        case SHADOW_ADDITIVE:
            Store(SHADOW_TEX_CAR, gpShadowPedTex);
            break;
        case SHADOW_INVCOLOR:
            Store(SHADOW_TEX_PED, gpShadowExplosionTex);
            break;
        case SHADOW_OIL_1:
            Store(SHADOW_TEX_CAR, gpShadowHeliTex);
            break;
        case SHADOW_OIL_2:
            Store(SHADOW_TEX_PED, gpShadowHeadLightsTex);
            break;
        case SHADOW_OIL_3:
            Store(SHADOW_TEX_CAR, gpBloodPoolTex);
            break;
        default:
            return;
    }
}

void CStaticShadow::Free() {
    if (m_pPolyBunch) {
        const auto prevHead = CShadows::pEmptyBunchList;
        CShadows::pEmptyBunchList = m_pPolyBunch;

        // Find last in the list and make it point to the previous head
        auto it{ m_pPolyBunch };
        while (it->m_pNext) {
            it = static_cast<CPolyBunch*>(it->m_pNext);
        }
        it->m_pNext = prevHead;

        m_pPolyBunch = nullptr;
        m_nId = 0;
    }
}

void CShadows::Shutdown() {
    RwTextureDestroy(gpShadowCarTex);
    RwTextureDestroy(gpShadowPedTex);
    RwTextureDestroy(gpShadowHeliTex);
    RwTextureDestroy(gpShadowBikeTex);
    RwTextureDestroy(gpShadowBaronTex);
    RwTextureDestroy(gpShadowExplosionTex);
    RwTextureDestroy(gpShadowHeadLightsTex);
    RwTextureDestroy(gpShadowHeadLightsTex2);
    RwTextureDestroy(gpBloodPoolTex);
    RwTextureDestroy(gpHandManTex);
    RwTextureDestroy(gpCrackedGlassTex);
    RwTextureDestroy(gpPostShadowTex);
}

void CShadows::TidyUpShadows() {
    for (auto& shadow : aPermanentShadows) {
        shadow.m_nType = SHADOW_NONE;
    }
}

void CShadows::SetRenderModeForShadowType(eShadowType type) {
    switch (type) {
        case SHADOW_DEFAULT:
        case SHADOW_OIL_1:
        case SHADOW_OIL_2:
        case SHADOW_OIL_3:
            /* case SHADOW_OIL_4: */ // missing
        case SHADOW_OIL_5:
            RwRenderStateSet(rwRENDERSTATESRCBLEND,  RWRSTATE(rwBLENDSRCALPHA));
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, RWRSTATE(rwBLENDINVSRCALPHA));
            break;
        case SHADOW_ADDITIVE:
            RwRenderStateSet(rwRENDERSTATESRCBLEND,  RWRSTATE(rwBLENDONE));
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, RWRSTATE(rwBLENDONE));
            break;
        case SHADOW_INVCOLOR:
            RwRenderStateSet(rwRENDERSTATESRCBLEND,  RWRSTATE(rwBLENDZERO));
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, RWRSTATE(rwBLENDINVSRCCOLOR));
            break;
        default:
            return;
    }
}

void CShadows::RemoveOilInArea(float x1, float x2, float y1, float y2) {
    CRect rect{ {x1, y1}, {x2, y2} };
    for (auto& shadow : aPermanentShadows) {
        switch (shadow.m_nType) {
            case SHADOW_OIL_1:
            case SHADOW_OIL_5:
                break;
            default:
                continue;
        }
        if (rect.IsPointInside(shadow.m_vecPosn)) {
            shadow.m_nType = SHADOW_NONE;
        }
    }
}

void CShadows::UpdateStaticShadows() {
    // Remove shadows that have no polies/are temporary and have expired
    for (auto& sshdw : aStaticShadows) {
        if (!sshdw.m_pPolyBunch || sshdw.m_bJustCreated) {
            goto skip; // Not even created fully
        }

        if (sshdw.m_bTemporaryShadow && CTimer::GetTimeInMS() <= sshdw.m_nTimeCreated + 5000u) {
            goto skip; // Not expired yet
        }

        sshdw.Free();

        skip:
        sshdw.m_bJustCreated = false;
    }
}



bool CShadows::StoreStaticShadow(uint32 id, eShadowType type, RwTexture* texture, const CVector* posn, float frontX, float frontY, float sideX, float sideY, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistane, float scale, float drawDistance, bool temporaryShadow, float upDistance) {
    return CHook::CallFunction<bool>(g_libGTASA + (VER_x32 ? 0x005B8D38 + 1 : 0x6DD6A4), id, type, texture, posn, frontX, frontY, sideX, sideY, intensity, red, green, blue, zDistane, scale, drawDistance, temporaryShadow, upDistance);
}

void CShadows::InjectHooks() {
    //CHook::Write(g_libGTASA + (VER_x32 ? 0x677BE4 : 0x84D7F8), &asShadowsStored);
    //CHook::Write(g_libGTASA + (VER_x32 ? 0x679914 : 0x851250), &ShadowsStoredToBeRendered);
//    CHook::Redirect("_ZN8CShadows4InitEv", &CShadows::Init);
    //CHook::Redirect("_ZN8CShadows19StoreCarLightShadowEP8CVehicleiP9RwTextureP7CVectorffffhhhf", &StoreCarLightShadow);
}
