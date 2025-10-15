//
// Created by x1y2zpenka on 05.09.2023.
//

#include "AdjustableHUD.h"
#include "../../vendor/armhook/patch.h"
#include "TouchInterface.h"

void CAdjustableHUD::SaveToDisk() {
    CHook::CallFunction<void>("_ZN14CAdjustableHUD10SaveToDiskEv", this);
}

CAdjustableHUD::~CAdjustableHUD() {
    SaveToDisk();
}


void (*CAdjustableHUD__Destructor)(CAdjustableHUD* thiz);
void CAdjustableHUD__Destructor_hook(CAdjustableHUD* thiz) {
    FLog("CAdjustableHUD::Destructor()");
//  //  delete thiz;
//    thiz->SaveToDisk();
//
//    auto phoneWidget = CTouchInterface::m_pWidgets[WidgetIDs::WIDGET_PHONE];
//    auto pos = ImVec2(phoneWidget->m_fOriginX, phoneWidget->m_fOriginY);
//    auto size = ImVec2(phoneWidget->m_fScaleX, phoneWidget->m_fScaleY);
//    MicroIcon::m_vecPos = CGUI::GegPosFromGta(pos);
//    MicroIcon::m_vecSize = ImVec2(size.x, size.y) * 7;
//    //MicroIcon::m_vecPos = CGUI::GegPosFromGta(pos);
//    auto newSize = ConvertGtaScaleToImguiSize(ImVec2(phoneWidget->m_fScaleX, phoneWidget->m_fScaleY));
//    CChatWindow::AddMessage("scale = %.2f, %.2f, size = %.2f, %.2f",
//                                 phoneWidget->m_fScaleX, phoneWidget->m_fScaleY,
//                                 size.x, size.y);
//    CChatWindow::AddMessage("rect = %.2f, %.2f -- %.2f, %.2f",
//                                 phoneWidget->m_RectPaddedScreen.left, phoneWidget->m_RectPaddedScreen.top,
//                                 phoneWidget->m_RectPaddedScreen.right, phoneWidget->m_RectPaddedScreen.bottom);
    // CAdjustableHUD__Destructor(thiz);
}

bool (*CAdjustableHUD__IsValidPosition)(CAdjustableHUD* thiz);
bool CAdjustableHUD__IsValidPosition_hook(CAdjustableHUD* thiz) {
    return true;
}

void CAdjustableHUD::InjectHooks() {
    //CHook::InlineHook("_ZN14CAdjustableHUD15IsValidPositionERK9CVector2DfPS0_", &CAdjustableHUD__IsValidPosition_hook, &CAdjustableHUD__IsValidPosition);
 //   CHook::InlineHook(g_libGTASA,  0x0024BB0C, &CAdjustableHUD__Destructor_hook, &CAdjustableHUD__Destructor);
}

