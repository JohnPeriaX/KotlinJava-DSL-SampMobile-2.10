//
// Created by x1y2z on 01.05.2023.
//

#pragma once

#include "WidgetRegion.h"

struct CWidgetRegionLook : CWidgetRegion
{
    bool m_bLookBack;
    bool m_bDisableLookBack;
    uint8_t pad1[2];
};

VALIDATE_SIZE(CWidgetRegionLook, (VER_x32 ? 0x94 : 0xA8));
