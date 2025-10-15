#pragma once

class CSnapShotHelper
{
public:
	CSnapShotHelper();

    RwTexture* CreatePedSnapShot(int iModel, uint32_t dwColor, CVector* vecRot, float fZoom);
    RwTexture* CreateVehicleSnapShot(int iModel, uint32_t dwColor, CVector* vecRot, float fZoom, uint32_t wColor1, uint32_t wColor2);
    RwTexture* CreateObjectSnapShot(int iModel, uint32_t dwColor, CVector* vecRot, float fZoom);

private:
	void SetUpScene();
	//void ProcessCamera(uintptr_t pRwObject, uint32_t dwColor);

	RwCamera* m_camera;
	RpLight* m_light;
	RwFrame* m_frame;
	RwRaster* m_zBuffer;
	uintptr_t m_raster;
};