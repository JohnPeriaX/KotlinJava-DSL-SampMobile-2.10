#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

extern CGame* pGame;
extern CNetGame* pNetGame;

// 0.3.7
CTextDrawPool::CTextDrawPool()
{
    for (int i = 0; i < MAX_TEXT_DRAWS; i++) {
        m_pTextDraw[i] = nullptr;
        m_bSlotState[i] = false;
    }

    ResetTextDrawTextures();

    // ====
    m_bSelectState = false;
    m_dwHoverColor = 0;
    m_wClickedTextDrawID = 0xFFFF;
}
// 0.3.7
CTextDrawPool::~CTextDrawPool()
{
    for (int i = 0; i < MAX_TEXT_DRAWS; i++) {
        Delete(i);
    }
}
// 0.3.7
void CTextDrawPool::New(uint16_t wTextDrawID, TEXT_DRAW_TRANSMIT* pTextDrawTransmit, const char* szText)
{
    if (m_pTextDraw[wTextDrawID]) {
        Delete(wTextDrawID);
    }

    CTextDraw* pTextDraw = new CTextDraw(pTextDrawTransmit, szText);
    if (pTextDraw == nullptr) return;

    m_pTextDraw[wTextDrawID] = pTextDraw;
    m_bSlotState[wTextDrawID] = true;
}
// 0.3.7
void CTextDrawPool::Delete(uint16_t wTextDrawID)
{
    if (m_pTextDraw[wTextDrawID]) {
        delete m_pTextDraw[wTextDrawID];
        m_pTextDraw[wTextDrawID] = nullptr;
        m_bSlotState[wTextDrawID] = false;
    }
}
// 0.3.7
void CTextDrawPool::Draw()
{
    for (int i = 0; i < MAX_TEXT_DRAWS; i++)
    {
        if (m_bSlotState[i]) {
            m_pTextDraw[i]->Draw();
        }
    }
}

void CTextDrawPool::DrawImage()
{
    /*for (int i = 0; i < MAX_TEXT_DRAWS; i++)
    {
        if (m_bSlotState[i]) {
            m_pTextDraw[i]->DrawImage();
        }
    }*/
}

// ========

void CTextDrawPool::SetSelectState(bool bState, uint32_t dwColor)
{
    if (bState) {
        m_bSelectState = true;
        m_dwHoverColor = (((dwColor << 16) | dwColor & 0xFF00) << 8) | (((dwColor >> 16) | dwColor & 0xFF0000) >> 8);
        m_wClickedTextDrawID = 0xFFFF;
        pGame->DisplayHUD(false);
        pGame->FindPlayerPed()->TogglePlayerControllable(false);
    }
    else {
        m_bSelectState = false;
        m_dwHoverColor = 0;
        m_wClickedTextDrawID = 0xFFFF;
        pGame->DisplayHUD(true);
        pGame->FindPlayerPed()->TogglePlayerControllable(true);

        SendClick();

        for (int i = 0; i < MAX_TEXT_DRAWS; i++)
        {
            if (m_bSlotState[i] && m_pTextDraw[i]) {
                CTextDraw* pTextDraw = m_pTextDraw[i];
                pTextDraw->m_bHovered = false;
                pTextDraw->m_dwHoverColor = 0;
            }
        }
    }
}

void CTextDrawPool::SendClick()
{
    RakNet::BitStream bsClick;
    bsClick.Write(m_wClickedTextDrawID);
    pNetGame->GetRakClient()->RPC(&RPC_ClickTextDraw, &bsClick, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

bool CTextDrawPool::onTouchEvent(int type, bool multi, int x, int y)
{
    if (m_bSelectState == false) return true;
    static bool bWannaClick = false;

    m_wClickedTextDrawID = 0xFFFF;


    for (int i = 0; i < MAX_TEXT_DRAWS; i++)
    {
        if (m_bSlotState[i] && m_pTextDraw[i])
        {
            CTextDraw* pTextDraw = m_pTextDraw[i];
            pTextDraw->m_bHovered = false;
            pTextDraw->m_dwHoverColor = 0;

            if (pTextDraw->m_TextDrawData.byteSelectable)
            {
                FLog("onTouchEvent textdraw 2");
                switch (type)
                {
                    case 2:
                        if (IsPointInRect(x, y, &(pTextDraw->m_rectArea)))
                        {
                            bWannaClick = true;
                            return false;
                        }
                        break;
                    case 3:
                        /*m_wClickedTextDrawID = 0xFFFF;
                        pTextDraw->m_bHovered = true;
                        pTextDraw->m_dwHoverColor = m_dwHoverColor;
                        FLog("x: %f, y:%f, %d",IsPointInRect(x, y, &pTextDraw->m_rectArea));*/
                        break;

                    case 1:
                        FLog("m_rectArea %f %f %f %f, clicked: %f %f", pTextDraw->m_rectArea.bottom, pTextDraw->m_rectArea.top,pTextDraw->m_rectArea.left,pTextDraw->m_rectArea.right,
                             x,y);
                        if (IsPointInRect(x, y, &pTextDraw->m_rectArea)) {
                            FLog("Point is in rect");
                            m_wClickedTextDrawID = i;
                            SendClick();
                            pTextDraw->m_bHovered = false;
                            pTextDraw->m_dwHoverColor = 0;
                            return false;
                        }
                        bWannaClick = false;
                        break;
                }
            }
        }
    }

    return true;
}

void CTextDrawPool::SnapshotProcess()
{
    for (int i = 0; i < MAX_TEXT_DRAWS; i++)
    {
        if (m_bSlotState[i] && m_pTextDraw[i]) {
            m_pTextDraw[i]->SnapshotProcess();
        }
    }
}