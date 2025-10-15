//
// Created by x1y2zpenka on 05.09.2023.
//

#pragma once


#include "TouchInterface.h"
#include "../Core/Rect.h"

struct __attribute__((aligned(4))) AdjustableButton
{
    WidgetIDs m_WidgetID;
    CTouchInterface::WidgetPositionIDs m_WidgetPositionID;
    CRect m_RectScreen;
    CSprite2d m_SpriteBackground;
    CSprite2d m_Sprite;
    int m_nAdjustmentModeFlags;
    bool m_bShownAsRed;
    bool m_bEnabled;
    uint8 pad0[2];
};
VALIDATE_SIZE(AdjustableButton, (VER_x32 ? 0x28 : 0x30));
