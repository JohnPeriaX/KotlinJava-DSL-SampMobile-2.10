//
// Created by plaka on 18.10.2023.
//

#include "ShadowCamera.h"

void CShadowCamera::Destroy() {
    if (!m_pRwCamera) {
        return;
    }

    if (auto frame = RwCameraGetFrame(m_pRwCamera)) {
        rwObjectHasFrameSetFrame(frame, nullptr);
        RwFrameDestroy(frame);
    }

    if (auto frameBuffer = GetRwRenderRaster()) {
        RwRasterDestroy(frameBuffer);
        RwCameraSetRaster(m_pRwCamera, nullptr);
    }

    if (m_pRwRenderTexture) {
        RwTextureSetRaster(m_pRwRenderTexture, nullptr);
        RwTextureDestroy(m_pRwRenderTexture);
        m_pRwRenderTexture = nullptr;
    }

    RwCameraDestroy(m_pRwCamera);
    m_pRwCamera = nullptr;
}

void CShadowCamera::SetFrustum(float radius) {
    RwCameraSetFarClipPlane(m_pRwCamera, 2 * radius);
    RwCameraSetNearClipPlane(m_pRwCamera, radius / 1000.0f);

    const CVector2D window(radius, radius);
    RwCameraSetViewWindow(m_pRwCamera, &window);
}

void CShadowCamera::InvertRaster() {
    const auto& raster = GetRwRenderRaster();
    const auto  width = (float)RwRasterGetWidth(raster), height = (float)RwRasterGetHeight(raster);

    // Helper to construct vertices used here
    const auto MkVert = [rhw = 1.f / RwCameraGetNearClipPlane(m_pRwCamera)](float x, float y) {
        return RwIm2DVertex{
                .x = x,
                .y = y,
                .z = RwIm2DGetNearScreenZ(),

                .rhw = rhw,

                .emissiveColor = 0xFFFFFFFF
        };
    };

    RwIm2DVertex vertices[]{
            MkVert(0.f,   0.f),    // Top left
            MkVert(0.f,   height), // Bottom left
            MkVert(width, 0.f),    // Top right
            MkVert(width, height), // Bottom right
    };

    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, RWRSTATE(NULL));
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, RWRSTATE(TRUE));
    RwRenderStateSet(rwRENDERSTATESRCBLEND, RWRSTATE(rwBLENDINVDESTCOLOR));
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, RWRSTATE(rwBLENDZERO));

    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vertices, std::size(vertices));

    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, RWRSTATE(TRUE));
    RwRenderStateSet(rwRENDERSTATESRCBLEND, RWRSTATE(rwBLENDSRCALPHA));
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, RWRSTATE(rwBLENDINVSRCALPHA));
}

void CShadowCamera::SetLight(RpLight *light) {
    auto* lightFrame  = RpLightGetFrame(light);
    auto* lightMatrix = RwFrameGetMatrix(lightFrame);
    auto* cameraFrame = RwCameraGetFrame(m_pRwCamera);
    cameraFrame->modelling.right = lightMatrix->right;
    cameraFrame->modelling.up = lightMatrix->up;
    cameraFrame->modelling.at = lightMatrix->at;
    RwMatrixUpdate(&cameraFrame->modelling);
    RwFrameUpdateObjects(cameraFrame);
}

RwCamera* CShadowCamera::Create(int32 rasterSizePower) {
    const auto rasterSizeInPx = 1 << rasterSizePower; // Size of raster in pixels. Same as `pow(2, rasterSize)`

    m_pRwCamera = RwCameraCreate();
    if (!m_pRwCamera) {
        fail:
        Destroy();
        return nullptr;
    }

    const auto frame = RwFrameCreate();
    if (!frame) {
        goto fail;
    }

    rwObjectHasFrameSetFrame(m_pRwCamera, frame);

    if (!RwCameraGetFrame(m_pRwCamera)) {
        goto fail;
    }

    const auto raster = RwRasterCreate(rasterSizeInPx, rasterSizeInPx, 0, rwRASTERTYPECAMERATEXTURE);
    if (!raster) {
        goto fail;
    }

    RwCameraSetRaster(m_pRwCamera, raster);

    m_pRwRenderTexture = RwTextureCreate(raster);
    if (!m_pRwRenderTexture) {
        goto fail;
    }

    RwTextureSetAddressing(m_pRwRenderTexture, rwFILTERMIPNEAREST | rwFILTERMIPNEAREST);
    RwTextureSetFilterMode(m_pRwRenderTexture, rwFILTERLINEAR);

    RwCameraSetProjection(m_pRwCamera, rwPARALLEL);

    return m_pRwCamera;
}


RwRaster* CShadowCamera::GetRwRenderRaster() const {
    return RwCameraGetRaster(m_pRwCamera);
}

RwTexture* CShadowCamera::GetRwRenderTexture() const {
    return m_pRwRenderTexture;
}

RwRaster* CShadowCamera::DrawOutlineBorder(const CRGBA& color) {
    // Helper to construct vertices used here
    const auto MkVert = [
            &color,
            rhw = 1.f / RwCameraGetNearClipPlane(m_pRwCamera)
    ](float x, float y) {
        RwIm2DVertex vtx;

        RwIm2DVertexSetScreenX(&vtx, x);
        RwIm2DVertexSetScreenY(&vtx, y);
        RwIm2DVertexSetScreenZ(&vtx, RwIm2DGetNearScreenZ());

        RwIm2DVertexSetRecipCameraZ(&vtx, rhw);

        RwIm2DVertexSetIntRGBA(&vtx, color.r, color.g, color.b, color.a);

        return vtx;
    };

    // NOTE: Code seemingly assumes the aspect ratio of the image is 1:1
    const auto w = RwRasterGetWidth(GetRwRenderRaster()) - 1.f;
    RwIm2DVertex vertices[]{
            MkVert(0.f, 0.f),
            MkVert(w,   0.f),
            MkVert(w,   w),
            MkVert(0.f, w),
    };

    if (RwCameraBeginUpdate(m_pRwCamera)) {
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE,       RWRSTATE(FALSE));
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, RWRSTATE(FALSE));
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER,     RWRSTATE(NULL));

        RwImVertexIndex indices[]{0, 1, 2, 3, 0};
        VERIFY(RwIm2DRenderIndexedPrimitive(rwPRIMTYPEPOLYLINE, vertices, std::size(vertices), indices, std::size(indices)));

        RwRenderStateSet(rwRENDERSTATEZTESTENABLE,       RWRSTATE(TRUE));
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, RWRSTATE(FALSE));

        RwCameraEndUpdate(m_pRwCamera);
    }

    return GetRwRenderRaster();
}
