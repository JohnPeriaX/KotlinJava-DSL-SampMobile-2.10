#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

extern CGame* pGame;

// 0.3.7
CGangZonePool::CGangZonePool()
{
	for (uint16_t wZone = 0; wZone < MAX_GANG_ZONES; wZone++)
	{
		m_pGangZone[wZone] = nullptr;
		m_bSlotState[wZone] = false;
	}
}
// 0.3.7
CGangZonePool::~CGangZonePool()
{
	for (uint16_t wZone = 0; wZone < MAX_GANG_ZONES; wZone++)
	{
		if (m_pGangZone[wZone])
		{
			delete m_pGangZone[wZone];
			m_pGangZone[wZone] = nullptr;
		}
	}
}
// 0.3.7
void CGangZonePool::New(uint16_t wZoneID, float fMinX, float fMinY, float fMaxX, float fMaxY, uint32_t dwColor)
{
	if (m_pGangZone[wZoneID])
	{
		delete m_pGangZone[wZoneID];
		m_pGangZone[wZoneID] = nullptr;
		m_bSlotState[wZoneID] = false;
	}

	GANG_ZONE *pGangZone = (GANG_ZONE*)malloc(sizeof(GANG_ZONE));
	if (pGangZone)
	{
		pGangZone->fPos[0] = fMinX;
		pGangZone->fPos[1] = fMaxY;
		pGangZone->fPos[2] = fMaxX;
		pGangZone->fPos[3] = fMinY;
		pGangZone->dwColor = dwColor;
		pGangZone->dwAltColor = dwColor;

		m_pGangZone[wZoneID] = pGangZone;
		m_bSlotState[wZoneID] = true;
	}
}
// 0.3.7
void CGangZonePool::Delete(uint16_t wZoneID)
{
	if (m_pGangZone[wZoneID])
	{
		delete m_pGangZone[wZoneID];
		m_pGangZone[wZoneID] = nullptr;
		m_bSlotState[wZoneID] = false;
	}
}
// 0.3.7
void CGangZonePool::Flash(uint16_t wZoneID, uint32_t dwColor)
{
	if (m_pGangZone[wZoneID]) {
		m_pGangZone[wZoneID]->dwAltColor = dwColor;
	}
}
// 0.3.7
void CGangZonePool::StopFlash(uint16_t wZoneID)
{
	if (m_pGangZone[wZoneID]) {
		m_pGangZone[wZoneID]->dwAltColor = m_pGangZone[wZoneID]->dwColor;
	}
}
// 0.3.7
void CGangZonePool::Draw(uint32_t unk)
{
	static uint32_t dwLastTick = 0;
	uint32_t dwTick = GetTickCount();
	static uint8_t alt = 0;
	if (dwTick - dwLastTick >= 500)
	{
		alt = ~alt;
		dwLastTick = dwTick;
	}

	for (uint16_t wZone = 0; wZone < MAX_GANG_ZONES; wZone++)
	{
		if (m_bSlotState[wZone]) {
			pGame->DrawGangZone(m_pGangZone[wZone]->fPos, 
				alt ? m_pGangZone[wZone]->dwAltColor : m_pGangZone[wZone]->dwColor, unk);
		}
	}
}