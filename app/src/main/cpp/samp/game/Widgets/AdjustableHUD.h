//
// Created by x1y2zpenka on 05.09.2023.
//

#pragma once


#include "game/Core/Rect.h"
#include "AdjustableButton.h"

class CAdjustableHUD {
public:
    int m_nAdjustmentMode;
    CRect m_RectOnFoot;
    CRect m_RectInCar;
    CRect m_RectCancel;
    CRect m_RectSave;
    CRect m_RectRestoreDefaults;
    CRect m_RectOverlapDialog;
    int m_nInitialTouchID;
    AdjustableButton m_AdjustableButtons[19];
    CRect m_SavedButtonPositions[19];
    int m_nSelectedButton;
    bool m_bMovedButton;
    bool m_bOverlappingDialog;
    bool m_bQueuedExit;
    CVector2D m_vecDragOffset;
    CVector2D m_vecDragAnchor;
    CVector2D m_vecCoords;

public:
    static void InjectHooks();

    CAdjustableHUD();
    ~CAdjustableHUD();

    void SaveToDisk();
};
VALIDATE_SIZE(CAdjustableHUD, (VER_x32 ? 0x4B0 : 0x548));
