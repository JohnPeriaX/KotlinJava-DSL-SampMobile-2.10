//
// Created by x1y2z on 03.02.2023.
//

#include "WidgetGta.h"
#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "vendor/armhook/patch.h"
#include "WidgetRegionLook.h"
#include "TouchInterface.h"
#include "WidgetButton.h"

extern CNetGame *pNetGame;
extern CGame *pGame;

CWidgetGta* m_pWidgets[WidgetIDs::NUM_WIDGETS];

enum eWidgetState {
    STATE_NONE,
    STATE_FIXED
};

WidgetIDs GetWidgetTypeFromWidget(CWidgetGta* pWidget)
{
    if(pWidget)
    {
        if(m_pWidgets[WidgetIDs::WIDGET_ATTACK] && pWidget == m_pWidgets[WidgetIDs::WIDGET_ATTACK]) return WidgetIDs::WIDGET_ATTACK;
        if(m_pWidgets[WidgetIDs::WIDGET_SPRINT] && pWidget == m_pWidgets[WidgetIDs::WIDGET_SPRINT]) return WidgetIDs::WIDGET_SPRINT;
        if(m_pWidgets[WidgetIDs::WIDGET_ACCELERATE] && pWidget == m_pWidgets[WidgetIDs::WIDGET_ACCELERATE]) return WidgetIDs::WIDGET_ACCELERATE;
        if(m_pWidgets[WidgetIDs::WIDGET_ENTER_CAR] && pWidget == m_pWidgets[WidgetIDs::WIDGET_ENTER_CAR]) return WidgetIDs::WIDGET_ENTER_CAR;
        if(m_pWidgets[WidgetIDs::WIDGET_BRAKE] && pWidget == m_pWidgets[WidgetIDs::WIDGET_BRAKE]) return WidgetIDs::WIDGET_BRAKE;
    }

    return static_cast<WidgetIDs>(-1);
}

void SetWidgetFromId(int idWidget, CWidgetGta* pWidget)
{
    m_pWidgets[idWidget] = pWidget;
}

void SetWidgetFromName(const char* name, CWidgetGta* pWidget)
{
    if(!strcmp("accelerate", name)) SetWidgetFromId(WidgetIDs::WIDGET_ACCELERATE, pWidget);
    if(!strcmp("hud_car", name)) SetWidgetFromId(WidgetIDs::WIDGET_ENTER_CAR, pWidget);
    if(!strcmp("brake", name)) SetWidgetFromId(WidgetIDs::WIDGET_BRAKE, pWidget);
}

eWidgetState ProcessFixedWidget(CWidgetGta* pWidget)
{
    WidgetIDs widgetType = GetWidgetTypeFromWidget(pWidget);

    CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
    switch(widgetType)
    {
        case -1:
            return STATE_NONE;
        case WidgetIDs::WIDGET_ATTACK:
        case WidgetIDs::WIDGET_SPRINT:
            if(pPlayerPed->IsInVehicle() ||
               pPlayerPed->IsInJetpackMode())
            {
                return STATE_FIXED;
            }
            break;
        case WidgetIDs::WIDGET_ACCELERATE:
        case WidgetIDs::WIDGET_BRAKE:
            if(!pPlayerPed->IsInVehicle() &&
               !pPlayerPed->IsInJetpackMode())
            {
                return STATE_FIXED;
            }
            break;
        case WidgetIDs::WIDGET_ENTER_CAR:
            if(pPlayerPed->IsInJetpackMode()) return STATE_NONE;

            if(pNetGame)
            {
                CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
                if(pVehiclePool)
                {
                    VEHICLEID vehicleId = pVehiclePool->FindNearestToLocalPlayerPed();
                    if(vehicleId == INVALID_VEHICLE_ID) return STATE_FIXED;

                    if(vehicleId != INVALID_VEHICLE_ID)
                    {
                        CVehicle *pVehicle = pVehiclePool->GetAt(vehicleId);
                        if(pVehicle)
                        {
                            if(!pPlayerPed->IsInVehicle() &&
                               pVehicle->m_pVehicle->GetDistanceFromLocalPlayerPed() > 10.0f)
                            {
                                return STATE_FIXED;
                            }
                        }
                    }
                }
            }
            break;
    }

    return STATE_NONE;
}

void CWidgetGta::SetEnabled(bool bEnabled) {
    m_bEnabled = bEnabled;
}

bool (*CWidget__IsTouched)(uintptr_t *thiz, CVector2D *pVecOut);
bool CWidget__IsTouched_hook(uintptr_t *thiz, CVector2D *pVecOut) {
//    if(*thiz == CWidgetGta::pWidgets[WIDGET_POSITION_HORN]) {
//        return true;
//    }
//    if(!CHUD::bIsShow)
//        return false;


    return CWidget__IsTouched(thiz, pVecOut);
}

uintptr_t (*CWidget)(CWidgetButton* thiz, const char* name, uintptr_t* a3, int a4, uintptr_t* a5);
uintptr_t CWidget_hook(CWidgetButton* thiz, const char* name, uintptr_t*  a3, int a4, uintptr_t* a5)
{
    FLog("New Widget: \"%s\" 0x%X", name, thiz-g_libGTASA);

    SetWidgetFromName(name, thiz);
    return CWidget(thiz, name, a3, a4, a5);
}

void (*CWidget__SetEnabled)(CWidgetGta* pWidget, bool bEnabled);
void CWidget__SetEnabled_hook(CWidgetGta* pWidget, bool bEnabled)
{
    if(pNetGame)
    {
        switch(ProcessFixedWidget(pWidget))
        {
            case STATE_NONE: break;
            case STATE_FIXED:
                bEnabled = false;
                break;
        }
    }

    CWidget__SetEnabled(pWidget, bEnabled);
}

void (*CWidgetButton__Update)(CWidgetButton* thiz);
void CWidgetButton__Update_hook(CWidgetButton* thiz) {
    if(pNetGame)
    {
        switch(ProcessFixedWidget(thiz))
        {
            case STATE_NONE: break;
            case STATE_FIXED: return;
        }
    }

    CWidgetButton__Update(thiz);
}

void CWidgetGta::InjectHooks() {
    CHook::InstallPLT(g_libGTASA + (VER_x32 ? 0x00671424 : 0x842408), &CWidgetButton__Update_hook, &CWidgetButton__Update);
    CHook::InlineHook("_ZN13CWidgetButtonC2EPKcRK14WidgetPositionjj10HIDMapping", &CWidget_hook, &CWidget);
    CHook::InlineHook("_ZN7CWidget10SetEnabledEb", &CWidget__SetEnabled_hook, &CWidget__SetEnabled);
}

void CWidgetGta::SetTexture(const char *name) {
    m_Sprite.m_pTexture = CUtil::LoadTextureFromDB("mobile", name);
}

bool CWidgetGta::IsReleased(CVector2D *pVecOut) {
    return CHook::CallFunction<bool>(g_libGTASA + (VER_x32 ? 0x002B3484 + 1 : 0x372794), this, pVecOut);
}

bool CWidgetGta::IsTouched(CVector2D *pVecOut) {
    return CHook::CallFunction<bool>(g_libGTASA + (VER_x32 ? 0x002B3324 + 1 : 0x3725D0), this, pVecOut);
}


