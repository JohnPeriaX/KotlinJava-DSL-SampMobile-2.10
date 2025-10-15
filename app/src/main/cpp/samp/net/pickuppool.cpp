#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

extern CGame *pGame;
extern CNetGame *pNetGame;

// 0.3.7
CPickupPool::CPickupPool()
{
	memset(m_Pickups, 0, sizeof(m_Pickups));
	m_iPickupCount = 0;

	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		m_dwHnd[i] = 0;
		m_iTimer[i] = 0;
		m_dwGTAId[i] = 0xFFFFFFFF;
	}
}
// 0.3.7
CPickupPool::~CPickupPool()
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (m_dwHnd[i] != 0) {
			ScriptCommand(&destroy_pickup, m_dwHnd[i]);
		}
	}
}
// 0.3.7
void CPickupPool::Process()
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (m_dwHnd[i] != 0)
		{
			if (m_droppedWeapon[i].bDroppedWeapon || m_Pickups[i].iType == 14)
			{
				if (ScriptCommand(&is_pickup_picked_up, m_dwHnd[i]))
				{
					RakNet::BitStream bsPickup;
					if (m_droppedWeapon[i].bDroppedWeapon == false)
					{
						bsPickup.Write(i);
						pNetGame->GetRakClient()->RPC(&RPC_PickedUpPickup, &bsPickup, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
					}
					else
					{
						bsPickup.Write(m_droppedWeapon[i].PlayerID); 
						pNetGame->GetRakClient()->RPC(&RPC_PickedUpPickup, &bsPickup, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
					}
				}
			}
			else if(m_iTimer[i] > 0)
			{
				m_iTimer[i]--;
			}
		}
	}
}
// 0.3.7
void CPickupPool::New(PICKUP *pPickup, int iPickup)
{
	if (m_iPickupCount >= MAX_PICKUPS || iPickup < 0 || iPickup >= MAX_PICKUPS) return;

	if (m_dwHnd[iPickup] != 0) {
		ScriptCommand(&destroy_pickup, m_dwHnd[iPickup]);
	}

	memcpy(&m_Pickups[iPickup], pPickup, sizeof(PICKUP));
	m_droppedWeapon[iPickup].bDroppedWeapon = false;

	int dwGTAId = 0;
	m_dwHnd[iPickup] = pGame->CreatePickup(pPickup->iModel, pPickup->iType,
		pPickup->fX, pPickup->fY, pPickup->fZ, &dwGTAId);
	m_dwGTAId[iPickup] = dwGTAId;
	m_iPickupCount++;
}
// 0.3.7
void CPickupPool::Destroy(int iPickup)
{
	if (m_iPickupCount <= 0 || iPickup < 0 || iPickup >= MAX_PICKUPS) return;

	if (m_dwHnd[iPickup])
	{
		ScriptCommand(&destroy_pickup, m_dwHnd[iPickup]);
		m_dwHnd[iPickup] = 0;
		m_iTimer[iPickup] = 0;
		m_dwGTAId[iPickup] = 0xFFFFFFFF;
		m_iPickupCount--;
	}
}
// 0.3.7
int CPickupPool::GetIDFromGTAId(int dwGTAId)
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (m_dwGTAId[i] == dwGTAId) {
			return i;
		}
	}

	return -1;
}
// 0.3.7
void CPickupPool::PickedUp(int dwGTAId)
{
	int PickupID = GetIDFromGTAId(dwGTAId);
	if (PickupID != 0xFFFFFFFF && m_dwHnd[PickupID] != 0 && m_iTimer[PickupID] == 0)
	{
		if (m_droppedWeapon[PickupID].bDroppedWeapon) return;

		RakNet::BitStream bsPickup;
		bsPickup.Write(PickupID);
		pNetGame->GetRakClient()->RPC(&RPC_PickedUpPickup, &bsPickup, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
		m_iTimer[PickupID] = 15;
	}
}