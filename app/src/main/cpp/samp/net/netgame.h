#pragma once

#include "../vendor/raknet/RakClientInterface.h"
#include "../vendor/raknet/RakNetworkFactory.h"
#include "../vendor/raknet/PacketEnumerations.h"
#include "../vendor/raknet/StringCompressor.h"

#include "../vendor/raknet/SAMP/SAMPRPC.h"

typedef unsigned short PLAYERID;
typedef unsigned short VEHICLEID;
typedef unsigned short OBJECTID;

#define GAMESTATE_WAIT_CONNECT	1
#define GAMESTATE_CONNECTING	2
#define GAMESTATE_CONNECTED		5
#define GAMESTATE_AWAIT_JOIN	6
#define GAMESTATE_RESTARTING	11

#define MAX_PLAYERS				1004
#define MAX_ACTORS				1000
#define MAX_VEHICLES			2000
#define MAX_PLAYER_NAME			24
#define MAX_GANG_ZONES			1024
#define MAX_PICKUPS				4096
#define MAX_OBJECTS				1000
#define MAX_MAP_ICONS			100
#define MAX_TEXT_LABELS			2048
#define MAX_TEXT_DRAWS			3072

// SEND RATE TICKS
#define NETMODE_IDLE_ONFOOT_SENDRATE	80
#define NETMODE_NORMAL_ONFOOT_SENDRATE	30
#define NETMODE_IDLE_INCAR_SENDRATE		80
#define NETMODE_NORMAL_INCAR_SENDRATE	30
#define NETMODE_HEADSYNC_SENDRATE		1000
#define NETMODE_AIM_SENDRATE			100
#define NETMODE_FIRING_SENDRATE			30
#define LANMODE_IDLE_ONFOOT_SENDRATE	20
#define LANMODE_NORMAL_ONFOOT_SENDRATE	15
#define LANMODE_IDLE_INCAR_SENDRATE		30
#define LANMODE_NORMAL_INCAR_SENDRATE	15
#define NETMODE_SEND_MULTIPLIER			2
#define STATS_UPDATE_TICKS 1000 // 1 second

#include "localplayer.h"
#include "remoteplayer.h"
#include "playerpool.h"
#include "vehiclepool.h"
#include "gangzonepool.h"
#include "pickuppool.h"
#include "objectpool.h"
#include "textlabelpool.h"
#include "textdrawpool.h"
#include "actorpool.h"
#include "menupool.h"
#include "playerbubblepool.h"

class CNetGame
{
public:
	CNetGame(const char* szHostOrIp, int iPort,
		const char* szPlayerName, const char* szPass);
	~CNetGame();

	void Process();

	int GetGameState() { return m_iGameState; }
	void SetGameState(int iGameState) { m_iGameState = iGameState; }

	void ShutdownForGameModeRestart();
	void UpdatePlayerScoresAndPings();
	void InitGameLogic();

	void SetMapIcon(uint8_t byteIconID, float fPosX, float fPosY, float fPosZ, uint8_t byteType, uint32_t dwColor, uint8_t byteStyle);
	void DisableMapIcon(uint8_t byteIconID);


	char	m_szHostName[256 + 1];
	char	m_szHostOrIp[256 + 1];
	int		m_iPort;
	bool	m_bLanMode;

	struct NET_SETTINGS
	{
		bool		bUseCJWalk;
		int			iDeathDropMoney;
		bool		bAllowWeapons;
		float		fGravity;
		bool		bDisableInteriorEnterExits;
		int			iVehicleFriendlyFire;
		bool		byteHoldTime;
		bool		bInstagib;
		bool		bZoneNames;
		bool		bFriendlyFire;
		int			iSpawnsAvailable;
		float		fNameTagDrawDistance;
		bool		bManualVehicleEngineAndLight;
		uint8_t		byteWorldTime_Hour;
		uint8_t		byteWorldTime_Minute;
		uint8_t		byteWeather;
		bool		bNameTagLOS;
		int			iShowPlayerMarkers;
		float		fGlobalChatRadius;
		bool		bShowNameTags;
		bool		bLimitGlobalChatRadius;
		float		fWorldBounds[4];
	} *m_pNetSet;

	RakClientInterface* GetRakClient() { return m_pRakClient; }
	CPlayerPool* GetPlayerPool() { return m_pPools->pPlayerPool; }
	CVehiclePool* GetVehiclePool() { return m_pPools->pVehiclePool; }
	CGangZonePool* GetGangZonePool() { return m_pPools->pGangZonePool; }
	CPickupPool* GetPickupPool() { return m_pPools->pPickupPool; }
	CObjectPool* GetObjectPool() { return m_pPools->pObjectPool; }
	C3DTextLabelPool* GetTextLabelPool() { return m_pPools->pTextLabelPool; }
	CTextDrawPool* GetTextDrawPool() { return m_pPools->pTextDrawPool; }
	CActorPool* GetActorPool() { return m_pPools->pActorPool; }
	CMenuPool* GetMenuPool() { return m_pPools->pMenuPool; }
	CPlayerBubblePool* GetPlayerBubblePool() { return m_pPools->pPlayerBubblePool; }

	void SendDialogResponse(uint16_t wDialogID, uint8_t byteButtonID, uint16_t wListBoxItem, const char* szInput);
	void SendChatMessage(const char* szMsg);
	void SendChatCommand(const char* szMsg);

	void SendPrevClass();
	void SendSpawn();
	void SendNextClass();

    void Packet_CustomRPC(Packet *p);

private:
	void InitializePools();
	void UninitializePools();
	void UpdateNetwork();
	void ProcessPools();
	void ProcessLoadingScreen();
	void ProcessConnecting();

	void Packet_AuthKey(Packet* pkt);
	void Packet_ConnectAttemptFailed(Packet* pkt);
	void Packet_NoFreeIncomingConnections(Packet* pkt);
	void Packet_DisconnectionNotification(Packet* pkt);
	void Packet_ConnectionSucceeded(Packet* pkt);
	void Packet_FailedInitializeEncription(Packet* pkt);
	void Packet_ConnectionBanned(Packet* pkt);
	void Packet_InvalidPassword(Packet* pkt);
	void Packet_ConnectionLost(Packet* pkt);
	void Packet_PlayerSync(Packet* pkt);
	void Packet_VehicleSync(Packet* pkt);
	void Packet_PassengerSync(Packet* pkt);
	void Packet_AimSync(Packet* pkt);
	void Packet_BulletSync(Packet* pkt);
	void Packet_MarkerSync(Packet* pkt);
	void Packet_TrailerSync(Packet *pkt);

	/* voice */
	void Packet_VoiceChannelOpenReply(Packet* pkt);
	void Packet_VoiceData(Packet* pkt);

	void ResetVehiclePool();
	void ResetActorPool();
	void ResetTextDrawPool();
	void ResetGangZonePool();
	void Reset3DTextLabelPool();
	void ResetMapIcons();
	void ResetPickupPool();
	void ResetObjectPool();
	void ResetMenuPool();

	RakClientInterface *m_pRakClient;

	bool		m_bNameTagStatus;
	int			m_iGameState;
	int			m_dwLastConnectAttempt;
	uint32_t	m_dwMapIcon[MAX_MAP_ICONS];

	struct NET_POOLS
	{
		CPlayerPool			*pPlayerPool;
		CVehiclePool		*pVehiclePool;
		CGangZonePool		*pGangZonePool;
		CPickupPool			*pPickupPool;
		CObjectPool			*pObjectPool;
		C3DTextLabelPool	*pTextLabelPool;
		CTextDrawPool		*pTextDrawPool;
		CActorPool			*pActorPool;
		CMenuPool			*pMenuPool;
		CPlayerBubblePool	*pPlayerBubblePool;
	} *m_pPools;

    void Packet_UnoccupiedSync(Packet *pkt);
};