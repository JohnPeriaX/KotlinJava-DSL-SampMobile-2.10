#pragma once

#include <cstring>
#include "Entity/CVehicleGTA.h"
class CVehicle
{
public:
	CVehicle(int iType, float fX, float fY, float fZ, float fRotation, bool bPreloaded, bool bSiren);
	virtual ~CVehicle();

	int GetVehicleSubtype();
	void AddComponent(int iComponentID);
	void RemoveComponent(int iComponentID);
	void SetPaintJob(uint8_t bytePaintJobID);
	void SetColor(uint8_t iColor1, uint8_t iColor2);
	void LinkToInterior(int iInterior);
	void SetDamageStatus(uint32_t dwPanelDamageStatus, uint32_t dwDoorDamageStatus, uint8_t byteLightDamageStatus);
	void SetTireDamageStatus(uint8_t byteTireDamageStatus);
	void SetPlateText(const char* szPlateText) { strncpy(m_szPlateText, szPlateText, 32); }
	void SetZAngle(float fAngle);
	void ProcessMarkers();
	bool IsOccupied();

	void AttachTrailer();
	void DetachTrailer();
	void SetTrailer(CVehicle* pTrailer);

	bool IsRCVehicle();
	float GetHealth();
	void SetHealth(float fHealth);
	CVehicle* GetTrailer();
	CVehicle* GetTractor();
	float GetTrainSpeed();
	uint16_t GetHydraThrusters();
	float GetBikeLean();
	bool IsATrainPart();
	bool IsATowTruck();
	bool IsATrailer();
	bool VerifyInstance();
	bool IsDriverLocalPlayer();
	void SetInvulnerable(bool bInv);
	bool HasSunk();
	bool HasADriver();

	int GetEngineState() { return m_iEngineState; }
	int GetLightState() { return m_iLightState; }

	void ApplyEngineState(int iState);
	void ApplyLightState(int state);

	void SetDoorState(int state);
	void SetComponentOpenState(int iDoor, int iComponent, float fDoorOpenRatio);
	void SetAlarmState(uint16_t wAlarmState) { m_pVehicle->m_nVehicleFlags.bSirenOrAlarm = wAlarmState; }
	void OpenWindow(uint8_t component);
	void CloseWindow(uint8_t component);

	void RemoveEveryoneFromVehicle();

	void UpdateColor();
	bool UpdateLastDrivenTime();

public:
    CVehicleGTA* m_pVehicle;
	CVehicle* m_pTrailer;
    uintptr 		m_dwGTAId;
	bool m_bPreloaded;
	int m_iEngineState;
	int m_iLightState;
	bool m_bIsInvulnerable;
	bool m_bIsLocked;
	bool m_bDoorsLocked;
	uint8_t m_byteObjectiveVehicle;
	bool m_bSpecialMarkerEnabled;
	uint8_t m_byteColor1;
	uint8_t m_byteColor2;
	bool m_bHaveColor;
	bool m_bSiren;
	char m_szPlateText[32];
	uint32_t m_dwMarkerID;

	void UpdateDamageStatus(uint32_t dwPanelDamage, uint32_t dwDoorDamage, uint8_t byteLightDamage);
	void SetPanelDamageStatus(uint32_t dwPanelStatus);
	uint32_t GetPanelDamageStatus();
	void SetDoorDamageStatus(uint32_t dwDoorStatus);
	uint32_t GetDoorDamageStatus();
	void SetLightDamageStatus(uint8_t byteLightStatus);
	uint8_t GetLightDamageStatus();
	void SetWheelPoppedStatus(uint8_t byteWheelStatus);
	uint8_t GetWheelPoppedStatus();

    bool SirenEnabled();

	void EnableSiren(bool bState);

	bool IsLandingGearNotUp();

	bool			m_bHasBeenDriven;
	uint32_t		m_dwTimeSinceLastDriven;\
};