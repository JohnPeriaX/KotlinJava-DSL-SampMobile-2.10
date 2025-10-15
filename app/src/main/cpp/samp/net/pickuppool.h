#pragma once

#pragma pack(push, 1)
typedef struct _PICKUP
{
	int iModel;
	int iType;
	float fX;
	float fY;
	float fZ;
} PICKUP;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _DROPPED_WEAPON
{
	bool		bDroppedWeapon;
	PLAYERID	PlayerID;
} DROPPED_WEAPON;
#pragma pack(pop)

class CPickupPool
{
public:
	CPickupPool();
	~CPickupPool();

	void Process();

	void New(PICKUP *pPickup, int iPickup);
	void Destroy(int iPickup);
	void PickedUp(int dwGTAId);

private:
	int GetIDFromGTAId(int dwGTAId);

private:
	int				m_iPickupCount;
	uintptr_t		m_dwHnd[MAX_PICKUPS];
	int				m_dwGTAId[MAX_PICKUPS];
	int				m_iTimer[MAX_PICKUPS];
	DROPPED_WEAPON	m_droppedWeapon[MAX_PICKUPS];
	PICKUP			m_Pickups[MAX_PICKUPS];
};