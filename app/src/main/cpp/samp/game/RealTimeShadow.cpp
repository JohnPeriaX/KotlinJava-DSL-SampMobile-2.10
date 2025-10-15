//
// Created by x1y2z on 18.10.2023.
//

#include "RealTimeShadow.h"

void CRealTimeShadow::InjectHooks() {

}

RwFrame* CRealTimeShadow::SetLightProperties(float angle, float unused, bool doSetCamLight) {
    if (const auto frame = RpLightGetFrame(m_pLight)) {
        const RwV3d yaxis{ 0.f, 1.f, 0.f };
        RwFrameRotate(frame, &yaxis, angle, rwCOMBINEREPLACE);

        const RwV3d xaxis{ 1.f, 0.f, 0.f };
        RwFrameRotate(frame, &xaxis, angle, rwCOMBINEPOSTCONCAT);

        if (doSetCamLight) {
            m_camera->SetLight(m_pLight);
        }

        return frame;
    }
    return nullptr;
}

void CRealTimeShadow::Destroy() {
    m_pOwner = nullptr;
    m_nRwObjectType = (uint32)-1;
    if (m_pLight) {
        // This is so convoluted... Do we actually have to do it like this? Wouldnt a simple `RwFrameDestroy(RpLightGetFrame(m_pLight))` work?
        const auto frame = RpLightGetFrame(m_pLight);
        rwObjectHasFrameSetFrame(m_pLight, nullptr);
        RwFrameDestroy(frame);

        RpLightDestroy(m_pLight);

        m_pLight = nullptr;
    }
}

bool CRealTimeShadow::Create(bool isBlurred, int32 blurPasses, bool drawMoreBlur) {
    m_pLight = RpLightCreate(rpLIGHTDIRECTIONAL);
    if (!m_pLight) {
        return false;
    }

    const RwRGBAReal lightColor{ 0.8f, 0.8f, 0.8f, 0.f };
    RpLightSetColor(m_pLight, &lightColor);

    rwObjectHasFrameSetFrame(m_pLight, RwFrameCreate());

    if (m_camera->Create(7)) {
        m_nBlurPasses = blurPasses;
        m_bBlurred = isBlurred;
        m_bDrawMoreBlur = drawMoreBlur;
//        if (!isBlurred || m_blurCamera.Create(6)) {
//            m_camera.SetLight(m_pLight);
//            return true;
//        }
    }

    Destroy();
    return false;
}