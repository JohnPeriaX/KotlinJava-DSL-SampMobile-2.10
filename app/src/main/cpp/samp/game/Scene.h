/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "common.h"

class CScene {
public:
    RpWorld*  m_pRpWorld;
    RwCamera* m_pRwCamera;
};
VALIDATE_SIZE(CScene, (VER_x32 ? 0x8 : 0x10));

extern CScene Scene;