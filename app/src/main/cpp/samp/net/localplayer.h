#pragma once

// spectate
#define SPECTATE_TYPE_NONE						0
#define SPECTATE_TYPE_PLAYER					1
#define SPECTATE_TYPE_VEHICLE					2

// special action's
#define SPECIAL_ACTION_NONE						0
#define SPECIAL_ACTION_USEJETPACK				2
#define SPECIAL_ACTION_DANCE1					5
#define SPECIAL_ACTION_DANCE2					6
#define SPECIAL_ACTION_DANCE3					7
#define SPECIAL_ACTION_DANCE4					8
#define SPECIAL_ACTION_HANDSUP					10
#define SPECIAL_ACTION_USECELLPHONE				11
#define SPECIAL_ACTION_SITTING					12
#define SPECIAL_ACTION_STOPUSECELLPHONE			13
#define SPECIAL_ACTION_NIGHTVISION				14
#define SPECIAL_ACTION_THERMALVISION			15

// added in 0.3
#define SPECIAL_ACTION_DUCK 			1
#define SPECIAL_ACTION_ENTER_VEHICLE 	3
#define SPECIAL_ACTION_EXIT_VEHICLE 	4
#define SPECIAL_ACTION_DRINK_BEER		20
#define SPECIAL_ACTION_SMOKE_CIGGY		21
#define SPECIAL_ACTION_DRINK_WINE		22
#define SPECIAL_ACTION_DRINK_SPRUNK		23
#define SPECIAL_ACTION_PISSING			68

// added in 0.3e
#define SPECIAL_ACTION_CUFFED			24
// added in 0.3x
#define SPECIAL_ACTION_CARRY			25

#define BULLET_HIT_TYPE_NONE					0
#define BULLET_HIT_TYPE_PLAYER					1
#define BULLET_HIT_TYPE_VEHICLE					2
#define BULLET_HIT_TYPE_OBJECT					3
#define BULLET_HIT_TYPE_PLAYER_OBJECT			4

#define INVALID_PLAYER_ID	0xFFFF
#define NO_TEAM 0xFF

#define VOICE_CHANNEL_STATE_CLOSED		0
#define VOICE_CHANNEL_STATE_WAIT_ACCEPT	1
#define VOICE_CHANNEL_STATE_ACCEPTED	2

extern bool bNeedEnterVehicle;

#pragma pack(push, 1)
struct PLAYER_SPAWN_INFO
{
    uint8_t byteTeam;
    int iSkin;
    uint8_t unk;
    CVector vecPos;
    float fRotation;
    int iSpawnWeapons[3];
    int iSpawnWeaponsAmmo[3];
};
#pragma pack(pop)
VALIDATE_SIZE(PLAYER_SPAWN_INFO, 46);

#pragma pack(push, 1)
struct ONFOOT_SYNC_DATA
{
	uint16_t lrAnalog;				// +0
	uint16_t udAnalog;				// +2
	uint16_t wKeys;					// +4
	CVector vecPos;					// +6
	CQuaternion quat;				// +18
	uint8_t byteHealth;				// +34
	uint8_t byteArmour;				// +35
	uint8_t byteCurrentWeapon;		// +36
	uint8_t byteSpecialAction;		// +37
	CVector vecMoveSpeed;			// +38
	CVector vecSurfOffsets;			// +50
	uint16_t wSurfInfo;				// +62
	union {
		struct {
			uint16_t id;
			uint8_t  frameDelta;
			union {
				struct {
					bool    loop : 1;
					bool    lockX : 1;
					bool    lockY : 1;
					bool    freeze : 1;
					uint8_t time : 2;
					uint8_t _unused : 1;
					bool    regular : 1;
				};
				uint8_t value;
			} flags;
		} animation;
		struct {
			uint32_t  dwAnimation;
			//uint16_t  dwAnimationFlags;
		};
	};
};
#pragma pack(pop)
VALIDATE_SIZE(ONFOOT_SYNC_DATA, 68);

#pragma pack(push, 1)
struct SPECTATOR_SYNC_DATA
{
	uint16_t lrAnalog;
	uint16_t udAnalog;
	uint16_t wKeys;
	CVector vecPos;
};
#pragma pack(pop)
VALIDATE_SIZE(SPECTATOR_SYNC_DATA, 18);

#pragma pack(push, 1)
struct AIM_SYNC_DATA
{
	uint8_t		byteCamMode;
	CVector 	vecAimf;
	CVector 	vecAimPos;
	float 		fAimZ;
	uint8_t 	byteCamExtZoom : 6;
	uint8_t 	byteWeaponState : 2;
	uint8_t 	aspect_ratio;
};
#pragma pack(pop)
VALIDATE_SIZE(AIM_SYNC_DATA, 31);

#pragma pack(push, 1)
struct INCAR_SYNC_DATA
{
	VEHICLEID VehicleID;			// +0
	uint16_t lrAnalog;				// +2
	uint16_t udAnalog;				// +4
	uint16_t wKeys;					// +6
	CQuaternion quat;				// +8
	CVector vecPos;					// +24
	CVector vecMoveSpeed;			// +36
	float fCarHealth;				// +48
	uint8_t bytePlayerHealth;		// +52
	uint8_t bytePlayerArmour;		// +53
	uint8_t byteCurrentWeapon;		// +54
	uint8_t byteSirenOn;			// +55
	uint8_t byteLandingGearState;	// +56
	VEHICLEID TrailerID;			// +57

	union
	{
		uint32_t HydraThrustAngle;
		float fTrainSpeed;
	};
};
#pragma pack(pop)
VALIDATE_SIZE(INCAR_SYNC_DATA, 63);

#pragma pack(push, 1)
struct PASSENGER_SYNC_DATA
{
	VEHICLEID VehicleID;			// +0
	union {
		uint16_t DriveBySeatAdditionalKeyWeapon;
		struct
		{
			uint8_t byteSeatFlags : 2;
			uint8_t byteDriveBy : 2;
			uint8_t byteCurrentWeapon : 6;
			uint8_t AdditionalKey : 2;
		};
	};
	uint8_t bytePlayerHealth;		// +4
	uint8_t bytePlayerArmour;		// +5
	uint16_t lrAnalog;				// +6
	uint16_t udAnalog;				// +8
	uint16_t wKeys;					// +10
	CVector vecPos;					// +12
};
#pragma pack(pop)
VALIDATE_SIZE(PASSENGER_SYNC_DATA, 24);

#pragma pack(push, 1)
struct BULLET_SYNC_DATA
{
	uint8_t byteHitType;
	PLAYERID PlayerID;
	CVector vecOrigin;
	CVector vecPos;
	CVector vecOffset;
	uint8_t byteWeaponID;
};
#pragma pack(pop)
VALIDATE_SIZE(BULLET_SYNC_DATA, 40);

#pragma pack(push, 1)
struct TRAILER_SYNC_DATA
{
	VEHICLEID trailerID;
	CVector vecPos;
	CQuaternion quat;
	CVector vecMoveSpeed;
	CVector vecTurnSpeed;
};
#pragma pack(pop)
VALIDATE_SIZE(TRAILER_SYNC_DATA, 54);

#pragma pack(push, 1)
struct UNOCCUPIED_SYNC_DATA
{
	VEHICLEID vehicleId;			// +0
	uint8_t byteSeatId;				// +2
	CVector vecRoll;				// +3
	CVector vecDirection;			// +15
	CVector vecPos;					// +27
	CVector vecMoveSpeed;			// +39
	CVector vecTurnSpeed;			// +51
	float fCarHealth;				// +63
};
#pragma pack(pop)
VALIDATE_SIZE(UNOCCUPIED_SYNC_DATA, 67);

class CLocalPlayer
{
public:
    static inline int 	m_nPlayersInRange{};

public:
	bool				m_bWaitingForSpawnRequestReply;
	uint32_t			m_SpectateID;
	bool				m_bSpectateProcessed;
	uint8_t				m_byteSpectateMode;
	uint8_t				m_byteSpectateType;
	uint8_t				m_byteTeam;
	bool				m_bIsActive;
	int					m_iSelectedClass;
	VEHICLEID			m_LastVehicle;
	VEHICLEID			m_nLastVehicle;

	bool                m_bWasInCar;
    bool				m_bPassengerDriveByMode;

private:
	CPlayerPed*			m_pPlayerPed;

    uint32_t			m_dwLastStatsUpdateTick;
    uint32_t			m_dwLastSendTick;
    uint32_t    		m_dwLastUpdateOnFootData;
    uint32_t			m_dwLastSendAimSyncTick;
    uint32_t			m_dwLastSendSyncTick;
    uint32_t			m_dwLastUpdateInCarData;
    uint32_t			m_dwLastSendSpecTick;
    uint32_t			m_dwLastWeaponsUpdateTick;
    uint32_t 			m_dwLastPerformStuffAnimTick;
    uint32_t			m_dwLastAimSendTick;
    uint32_t			m_dwLastSendAimTick;
    uint32_t			m_dwLastHeadUpdate;
    uint32_t 			m_dwTimeLastSendOnFootSync;
    uint32_t 			m_dwLastUpdatePassengerData;

    PLAYER_SPAWN_INFO	m_SpawnInfo;
    ONFOOT_SYNC_DATA	m_LastSendOnFootSync{};
    INCAR_SYNC_DATA		m_InCarData;
    PASSENGER_SYNC_DATA	m_PassengerData;
    AIM_SYNC_DATA		m_aimSync;
    TRAILER_SYNC_DATA 	m_TrailerData;
    UNOCCUPIED_SYNC_DATA m_UnoccupiedData;

	bool				m_bDeathSended;

    uint32_t			m_dwPassengerEnterExit;

    bool				m_bWantsAnotherClass;

	uint8_t				m_byteLastWeapon[13];
	uint32_t			m_dwLastAmmo[13];

	bool				m_bIsWasted;
	bool				m_bClearedToSpawn;
	bool				m_bInRCMode;
    bool				m_bHasSpawnInfo;
	uint32_t			m_dwAnimation;
	bool				m_bSpawnDialogShowed;
	uint8_t				m_byteCurrentWeapon;
	
	bool				m_bIsSpectating;

	int					m_iDisplayZoneTick;

    uint8_t				m_byteCurInterior;

	VEHICLEID			m_CurrentVehicle;
    PLAYERID 			lastDamageId;
    eWeaponType 		lastDamageWeap;

	bool				m_bPerformingStuffAnim;

	struct {
		CVector vecOffsetPos;
		int dwSurfVehID;

		bool bIsActive;
		bool bIsVehicle;
		uintptr_t pSurfInst;
	} m_surfData;

	struct {
		uint32_t dwLastMoney;
		uint32_t dwLastDrunkLevel;
	} m_statsData;

private:
	void SendUnoccupiedData(VEHICLEID vehicleId, CVehicle *pVehicle);

	int GetOptimumUnoccupiedSendRate();

    bool EnterVehicleAsPassenger();

public:
	CLocalPlayer();
	~CLocalPlayer();

	static void SendDeath();
	void GoEnterVehicle(bool passenger);
	static uint32_t GetPlayerColor();

    uint32_t CalculateAimSendRate(uint16_t wKeys) const;

	bool Process();
	bool Spawn();
	void ResetAllSyncAttributes();

	void ToggleSpectating(bool bToggle);
	void ProcessSpectating();
	bool IsSpectating() { return m_bIsSpectating; }
	bool IsInRCMode() { return m_bInRCMode; };
	void SpectatePlayer(PLAYERID PlayerID);
	void SpectateVehicle(VEHICLEID VehicleID);

	void CheckWeapons();
	void UpdateRemoteInterior(uint8_t byteInterior);
	void UpdateCameraTarget();
	void ProcessSurfing();
	void UpdateSurfing();
	void UpdateVehicleDamage(VEHICLEID vehicleID);
	bool IsClearedToSpawn() { return m_bClearedToSpawn; }
	void HandleClassSelection();
	void MoveHeadWithCamera();

	bool HandlePassengerEntry();
	bool GbuttonEnterVehicleAsPassenger();
	bool EnterVehicleAsDriver();

	void ProcessOnFootWorldBounds();
	void ProcessInCarWorldBounds();
	bool CompareOnFootSyncKeys(uint16_t wKeys, uint16_t lrAnalog, uint16_t udAnalog);
	void ApplySpecialAction(uint8_t byteSpecialAction);
	void SetSpawnInfo(PLAYER_SPAWN_INFO *pSpawnInfo);
	void HandleClassSelectionOutcome(bool bOutcome);
	uint8_t GetSpecialAction();
	uint32_t GetPlayerColorAsARGB();
	uint32_t GetPlayerColorAsRGBA();

	void DisableSurf() { m_surfData.bIsActive = false; }

	void SetPlayerColor(uint32_t dwColor);

	int GetOptimumOnFootSendRate();
	int GetOptimumInCarSendRate();

	void SendPrevClass();
	void SendNextClass();
	void SendSpawn();
	void RequestClass(int iClass);
	void RequestSpawn();
	void ProcessClassSelection();

	void SendStatsUpdate();
	void SendWastedNotification();
	void SendOnFootFullSyncData();
	void SendInCarFullSyncData();
	void SendPassengerFullSyncData();
	void SendAimSyncData();

	bool IsNeedSyncDataSend(const void* data1, const void* data2, size_t size);

	void SendEnterVehicleNotification(VEHICLEID VehicleID, bool bPassenger);
	void SendExitVehicleNotification(VEHICLEID VehicleID);

	void SendTakeDamageEvent(PLAYERID PlayerID, float fDamageFactor, int weaponType, int pedPieceType);
	void SendGiveDamageEvent(PLAYERID PlayerID, float fDamageFactor, int weaponType, int pedPieceType);
	void SendGiveDamageActorEvent(PLAYERID ActorID, float fDamageFactor, int weaponType, int pedPieceType);

	CPlayerPed* GetPlayerPed() { return m_pPlayerPed; }

	/* voice */
	void UpdateVoice();
	void VoiceChannelAccept();
	void VoiceChannelClose();

	void SendVCData();
	void SendVCOpenRequest();
	void SendVCClose();

	void SendTrailerData(VEHICLEID vehicleId);

	void GiveTakeDamage(bool bGiveOrTake, uint16_t wPlayerID, float damage_amount, uint32_t weapon_id, uint32_t bodypart);

	uint32_t GetCurrentAnimationIndexFlag();

	void SendBulletSyncData(PLAYERID byteHitID, uint8_t byteHitType, CVector vecHitPos);

	void GiveActorDamage(PLAYERID wPlayerID, float damage_amount, uint32_t weapon_id, uint32_t bodypart);

	int					m_iVCState;
	uint32_t			m_dwVCOpenRequestTick;

	void SetTeam(uint8_t byteTeam) { m_byteTeam = byteTeam; };
	uint8_t GetTeam() { return m_byteTeam; };

	bool ProcessUnoccupiedSync(VEHICLEID vehicleId, CVehicle *pVehicle);

    void MaybeSendExitVehicle();
    void MaybeSendEnterVehicle();
};