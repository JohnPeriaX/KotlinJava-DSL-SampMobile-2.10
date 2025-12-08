#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

extern CGame* pGame;


CGangZonePool::CGangZonePool()
{
	for (uint16_t wZone = 0; wZone < MAX_GANG_ZONES; wZone++)
	{
		m_pGangZone[wZone] = nullptr;
		m_bSlotState[wZone] = false;
	}
}

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

void CGangZonePool::Delete(uint16_t wZoneID)
{
	if (m_pGangZone[wZoneID])
	{
		delete m_pGangZone[wZoneID];
		m_pGangZone[wZoneID] = nullptr;
		m_bSlotState[wZoneID] = false;
	}
}

void CGangZonePool::Flash(uint16_t wZoneID, uint32_t dwColor)
{
	if (m_pGangZone[wZoneID]) {
		m_pGangZone[wZoneID]->dwAltColor = dwColor;
	}
}

void CGangZonePool::StopFlash(uint16_t wZoneID)
{
	if (m_pGangZone[wZoneID]) {
		m_pGangZone[wZoneID]->dwAltColor = m_pGangZone[wZoneID]->dwColor;
	}
}

void CGangZonePool::Draw(uint32_t unk)
{
	static uint32_t dwLastTick = 0;
	uint32_t dwTick = CTimer::m_snTimeInMillisecondsNonClipped;
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