//
// Created by x1y2z on 25.01.2024.
//

#pragma once

#include "../sprite2d.h"
#include "WidgetGta.h"

class CWidgetRadar : public CWidgetGta {
    CSprite2d mSpriteCrossHairs;

public:
    static void InjectHooks();
};
VALIDATE_SIZE(CWidgetRadar, (VER_x32 ? 0x94 : 0xB0));

