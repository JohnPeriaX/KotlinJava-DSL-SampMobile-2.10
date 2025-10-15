//
// Created by x1y2z on 25.01.2024.
//

#include "WidgetRadar.h"
#include "vendor/armhook/patch.h"


void (*CWidgetRadar__Update)(CWidgetRadar* thiz);
void CWidgetRadar__Update_hook(CWidgetRadar* thiz)
{
    thiz->m_fTapHoldTime = 999.f;
    CWidgetRadar__Update(thiz);
}

void CWidgetRadar::InjectHooks() {
    //CHook::InlineHook("_ZN12CWidgetRadar10IsHeldDownEv", &CWidgetRadar__Update_hook, &CWidgetRadar__Update);
}
