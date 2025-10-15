#pragma once

#define MAX_TEXT_DRAW_LINE 800

class CTextDrawPool
{
public:
    CTextDrawPool();
    ~CTextDrawPool();

    void New(uint16_t wTextDrawID, TEXT_DRAW_TRANSMIT* pTextDrawTransmit, const char* szText);
    void Delete(uint16_t wTextDrawID);
    void Draw();
    void DrawImage();

    CTextDraw* GetAt(uint16_t wTextDrawID) {
        if (wTextDrawID >= MAX_TEXT_DRAWS) return nullptr;
        if (!m_bSlotState[wTextDrawID]) return nullptr;
        return m_pTextDraw[wTextDrawID];
    }

    bool GetState() {
        return m_bSelectState;
    }

    void SetSelectState(bool bState, uint32_t dwColor = 0);
    bool onTouchEvent(int type, bool multi, int x, int y);

    void SnapshotProcess();

private:
    void SendClick();

private:
    uint8_t m_bSlotState[MAX_TEXT_DRAWS];
    CTextDraw* m_pTextDraw[MAX_TEXT_DRAWS];

    bool m_bSelectState;
    uint32_t m_dwHoverColor;
    uint16_t m_wClickedTextDrawID;
};