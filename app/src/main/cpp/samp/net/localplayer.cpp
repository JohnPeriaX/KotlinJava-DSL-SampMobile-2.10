#include "../main.h"
#include "../game/game.h"
#include "netgame.h"
#include "localplayer.h"
#include "../gui/gui.h"
#include "../game/Timer.h"

// voice
#include "../voice_new/MicroIcon.h"
#include "../voice_new/SpeakerList.h"
#include "game/Tasks/TaskTypes/TaskComplexEnterCarAsDriver.h"

extern UI* pUI;
extern CGame *pGame;
extern CNetGame *pNetGame;
//extern CVoice* pVoice;

extern int iNetModeNormalOnFootSendRate;
extern int iNetModeNormalInCarSendRate;
extern int iNetModeFiringSendRate;
extern int iNetModeSendMultiplier;

//#define IS_TARGETING(x) ((x) & 128)
//#define IS_FIRING(x) ((x) & 4)

bool m_bWasInCar = false;

extern bool bUsedPlayerSlots[];

uint32_t dwEnterVehTimeElasped = -1;
bool bFirstSpawn = true;
bool g_bLockEnterVehicleWidget = false;
uint32_t count = 0;

bool IsLongTimeDead()
{
    if(pGame->FindPlayerPed()->IsDead()){
        if ((CTimer::m_snTimeInMillisecondsNonClipped-count) > 5000) {
            return true;
        }
    }
    return false;
}

CLocalPlayer::CLocalPlayer()
{
	m_pPlayerPed = pGame->FindPlayerPed();
	m_bDeathSended = false;

    m_iSelectedClass = 0;
    m_bHasSpawnInfo = false;
    m_bWaitingForSpawnRequestReply = false;
    m_bWantsAnotherClass = false;
	m_bInRCMode = false;

    m_surfData.bIsActive = false;
    m_surfData.pSurfInst = 0;
    m_surfData.bIsVehicle = false;
    m_surfData.vecOffsetPos = CVector {0.0f, 0.0f, 0.0f};

    m_statsData.dwLastMoney = 0;
    m_statsData.dwLastDrunkLevel = 0;

    m_bIsActive = false;
    m_bIsWasted = false;

    m_iDisplayZoneTick = 0;
	m_dwLastSendTick = CTimer::m_snTimeInMillisecondsNonClipped;
	m_dwLastSendAimTick = CTimer::m_snTimeInMillisecondsNonClipped;
	m_dwLastSendSpecTick = CTimer::m_snTimeInMillisecondsNonClipped;
    m_dwLastSendSyncTick = CTimer::m_snTimeInMillisecondsNonClipped;
    m_dwLastSendAimSyncTick = CTimer::m_snTimeInMillisecondsNonClipped;
	m_dwLastAimSendTick = m_dwLastSendTick;
	m_dwTimeLastSendOnFootSync = CTimer::m_snTimeInMillisecondsNonClipped;
	m_dwLastStatsUpdateTick = CTimer::m_snTimeInMillisecondsNonClipped;
	m_dwLastUpdateInCarData = CTimer::m_snTimeInMillisecondsNonClipped;
	m_dwLastUpdatePassengerData = CTimer::m_snTimeInMillisecondsNonClipped;
	m_dwPassengerEnterExit = CTimer::m_snTimeInMillisecondsNonClipped;
    m_dwLastPerformStuffAnimTick = CTimer::m_snTimeInMillisecondsNonClipped;
    m_dwLastUpdateOnFootData = CTimer::m_snTimeInMillisecondsNonClipped;

    count = CTimer::m_snTimeInMillisecondsNonClipped;

	m_CurrentVehicle = INVALID_VEHICLE_ID;
	ResetAllSyncAttributes();

    m_bIsSpectating = false;
    m_byteSpectateType = SPECTATE_TYPE_NONE;
    m_SpectateID = 0xFFFFFFFF;
    m_bSpawnDialogShowed = false;

	for (uint8_t i = 0; i < 13; i++)
	{
		m_byteLastWeapon[i] = 0;
		m_dwLastAmmo[i] = 0;
	}

    m_byteTeam = NO_TEAM;
}

CLocalPlayer::~CLocalPlayer()
= default;

void CLocalPlayer::ResetAllSyncAttributes()
{
    m_bHasSpawnInfo = false;
    m_bWaitingForSpawnRequestReply = false;
    m_iSelectedClass = 0;
    m_byteCurInterior = 0;
    m_LastVehicle = 0xFFFF;
    m_bInRCMode = false;

    memset(&m_SpawnInfo, 0, sizeof(PLAYER_SPAWN_INFO));
    memset(&m_LastSendOnFootSync, 0, sizeof(ONFOOT_SYNC_DATA));
    memset(&m_LastSendOnFootSync, 0, sizeof(INCAR_SYNC_DATA));
    memset(&m_TrailerData, 0, sizeof(TRAILER_SYNC_DATA));
    memset(&m_PassengerData, 0, sizeof(PASSENGER_SYNC_DATA));
    memset(&m_aimSync, 0, sizeof(AIM_SYNC_DATA));

    m_dwAnimation = 0;
    m_dwLastWeaponsUpdateTick = CTimer::m_snTimeInMillisecondsNonClipped;
    m_byteCurrentWeapon = 0;

    /* voice */
    m_iVCState = VOICE_CHANNEL_STATE_CLOSED;
    m_dwVCOpenRequestTick = CTimer::m_snTimeInMillisecondsNonClipped;

    m_CurrentVehicle = INVALID_VEHICLE_ID;
    m_LastVehicle = INVALID_VEHICLE_ID;
    m_nLastVehicle = INVALID_VEHICLE_ID;
    m_bWasInCar = false;
}

extern int g_iLagCompensationMode;
extern bool DriveBy;
bool g_uiHeadMoveEnabled;

CAMERA_AIM* caAim = new CAMERA_AIM();

CVector lastPos;

bool CLocalPlayer::Process()
{
    if (CTimer::GetIsUserPaused())
        return false;

    // Basic Updates
    lastPos = m_pPlayerPed->m_pPed->GetPosition();
    m_pPlayerPed->SetCurrentAim(caAim);
    caAim = m_pPlayerPed->GetCurrentAim();
    
    // Cache Time (Optimization)
    uint32_t dwThisTick = CTimer::m_snTimeInMillisecondsNonClipped;

    // Safety Check
    if (!m_pPlayerPed || !m_pPlayerPed->m_pPed) 
        return false;

    // SA-MP Specifics (Race & Drunk from)
    /*if (CMultiplayer::m_bRaceCheckpointsEnabled)
    {
        if (DistanceBetweenPoints2D(CMultiplayer::m_vecRaceCheckpointPos, m_pPlayerPed->m_pPed->GetPosition()) <= CMultiplayer::m_fRaceCheckpointSize) 
        {
            CMultiplayer::RaceCheckpointPicked();
        }
    }

    if (m_pPlayerPed->drunk_level > 0) 
    {
        m_pPlayerPed->drunk_level--;
        ScriptCommand(&SET_PLAYER_DRUNKENNESS, m_pPlayerPed->m_bytePlayerNumber, m_pPlayerPed->drunk_level / 100);
    }*/

    // Death Logic
    if (m_bIsActive)
    {
        // Check if dead or action is death
        if (!m_bIsWasted && (m_pPlayerPed->GetActionTrigger() == ACTION_DEATH || m_pPlayerPed->IsDead()))
        {
            ToggleSpectating(false);

            // Reset States
            if (m_pPlayerPed->GetDanceStyle() != -1) m_pPlayerPed->StopDancing();
            if (m_pPlayerPed->IsCellphoneEnabled()) m_pPlayerPed->ToggleCellphone(0);
            if (m_pPlayerPed->IsPissing()) m_pPlayerPed->StopPissing();
            if (m_pPlayerPed->GetStuff() != eStuffType::STUFF_TYPE_NONE) m_pPlayerPed->DropStuff();

            m_pPlayerPed->TogglePlayerControllable(true);

            // Handle RC Mode exit
            if (m_bInRCMode)
            {
                m_bInRCMode = false;
                m_pPlayerPed->m_pPed->Add();
            }

            // Sync last vehicle data if died inside
            if (m_pPlayerPed->IsInVehicle() && !m_pPlayerPed->IsAPassenger())
            {
                SendInCarFullSyncData(); 
                m_LastVehicle = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
            }

            m_pPlayerPed->ExtinguishFire();
            m_pPlayerPed->SetHealth(0.0f);
            m_pPlayerPed->SetDead();

            SendWastedNotification(); // Or SendDeath() depending on your netcode

            m_bIsActive = false;
            m_bIsWasted = true;

            pGame->EnableZoneNames(false);

            return true;
        }

        // Special Actions Handling
        uint16_t wKeys, lrAnalog, udAnalog;
        wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog, false);

        // Dancing
        if (m_pPlayerPed->GetDanceStyle() != -1)
        {
            m_pPlayerPed->ProcessDancing();
            if (LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] || m_pPlayerPed->IsInVehicle() || m_pPlayerPed->IsInJetpackMode())
                m_pPlayerPed->StopDancing();
        }

        // Hands Up
        if (m_pPlayerPed->HasHandsUp())
        {
            if (LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] || m_pPlayerPed->IsInVehicle() || m_pPlayerPed->IsInJetpackMode())
                m_pPlayerPed->TogglePlayerControllable(true);
        }

        // Cellphone
        if (m_pPlayerPed->IsCellphoneEnabled())
        {
            if (LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] || m_pPlayerPed->IsInVehicle() || m_pPlayerPed->IsInJetpackMode())
                m_pPlayerPed->ToggleCellphone(0);
        }

        // Pissing
        if (m_pPlayerPed->IsPissing())
        {
            if (LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] || m_pPlayerPed->IsInVehicle() || m_pPlayerPed->IsInJetpackMode())
                m_pPlayerPed->StopPissing();
        }

        // Stuff (Attached Objects)
        if (m_pPlayerPed->GetStuff() != eStuffType::STUFF_TYPE_NONE)
        {
            if ((dwThisTick - m_dwLastPerformStuffAnimTick) > 3500)
                m_bPerformingStuffAnim = false;

            if (!m_bPerformingStuffAnim && LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE])
            {
                if (m_pPlayerPed->ApplyStuff())
                {
                    m_dwLastPerformStuffAnimTick = dwThisTick;
                    m_bPerformingStuffAnim = true;
                }
            }

            if (LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] || m_pPlayerPed->IsInVehicle() || m_pPlayerPed->IsInJetpackMode())
                m_pPlayerPed->DropStuff();
        }

        m_pPlayerPed->ProcessDrunk();

        // Game Updates (Stats, Zones, Interiors)
        if (dwEnterVehTimeElasped != -1 && (dwThisTick - dwEnterVehTimeElasped) > 5000 && !m_pPlayerPed->IsInVehicle())
        {
            CCamera::SetBehindPlayer();
            dwEnterVehTimeElasped = -1;
        }

        if (dwThisTick >= m_iDisplayZoneTick) {
            pGame->EnableZoneNames(pNetGame->m_pNetSet->bZoneNames);
        }
        
        pGame->UpdateCheckpoints();

        if ((dwThisTick - m_dwLastStatsUpdateTick) > 1000) {
            SendStatsUpdate();
            m_dwLastStatsUpdateTick = dwThisTick;
        }

        UpdateSurfing();
        CheckWeapons();
        
        uint8_t byteInterior = pGame->GetActiveInterior();
        if (byteInterior != m_byteCurInterior) {
            UpdateRemoteInterior(byteInterior);
        }
        
        UpdateCameraTarget();

        // SYNC LOGIC START
        if (m_bIsSpectating) 
        {
            ProcessSpectating();
            m_bPassengerDriveByMode = false;
        }
        // DRIVER LOGIC
        else if (m_pPlayerPed->IsInVehicle() && !m_pPlayerPed->IsAPassenger())
        {
            MaybeSendExitVehicle();
            g_bLockEnterVehicleWidget = false;

            CVehicleGTA* pGtaVehicle = m_pPlayerPed->GetGtaVehicle();
            m_nLastVehicle = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(pGtaVehicle);

            MoveHeadWithCamera();
            ProcessInCarWorldBounds();

            // Aim Sync inside car (Important for Hydras/Tanks/Driveby)
            if ((dwThisTick - m_dwLastSendAimSyncTick) > 500)
            {
                m_dwLastSendAimSyncTick = dwThisTick;
                SendAimSyncData();
            }

            CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
            CVehicle *pVehicle = nullptr;
            if (pVehiclePool) {
                m_CurrentVehicle = pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
                pVehicle = pVehiclePool->GetAt(m_CurrentVehicle);
            }

            // RC Mode Handling
            if (pVehicle && !m_bInRCMode && pVehicle->IsRCVehicle())
            {
                m_pPlayerPed->m_pPed->Remove();
                m_bInRCMode = true;
            }
            if (m_bInRCMode && !pVehicle) // RC destroyed or lost
            {
                m_pPlayerPed->SetHealth(0.0f);
                m_pPlayerPed->SetDead();
            }
            if (m_bInRCMode && pVehicle && pVehicle->GetHealth() == 0.0f)
            {
                m_pPlayerPed->SetHealth(0.0f);
                m_pPlayerPed->SetDead();
            }

            // Sync Sending
            if ((dwThisTick - m_dwLastSendTick) > GetOptimumInCarSendRate())
            {
                m_dwLastSendTick = dwThisTick;
                SendInCarFullSyncData();
                UpdateVehicleDamage(m_CurrentVehicle);
            }

            m_bPassengerDriveByMode = false;
        }
        // ONFOOT LOGIC
        else if (m_pPlayerPed->GetActionTrigger() == ACTION_NORMAL || m_pPlayerPed->GetActionTrigger() == ACTION_SCOPE)
        {
            if ((dwThisTick - m_dwLastSendTick) > GetOptimumOnFootSendRate())
            {
                m_dwLastSendTick = dwThisTick;
                g_bLockEnterVehicleWidget = true;
                
                if (m_bWasInCar) m_bWasInCar = false;

                MoveHeadWithCamera();

                if (m_bInRCMode)
                {
                    m_bInRCMode = false;
                    m_pPlayerPed->m_pPed->Add();
                }

                HandlePassengerEntry();
                ProcessOnFootWorldBounds();

                if (m_CurrentVehicle != 0xFFFF)
                {
                    m_LastVehicle = m_CurrentVehicle;
                    m_CurrentVehicle = 0xFFFF;
                }

                ProcessSurfing();
                MaybeSendEnterVehicle();
                SendOnFootFullSyncData();
            }

            // Aim Sync Logic (Smart Rate)
            if ((dwThisTick - m_dwLastSendTick) < 1000)
            {
                bool bIsFiring = IS_TARGETING(m_pPlayerPed->m_pPed) && IS_FIRING(m_pPlayerPed->m_pPed);
                int iSyncRate = (g_iLagCompensationMode == 2 && bIsFiring) ? iNetModeFiringSendRate : 500;
                
                // Fallback rate if not targeting/firing but moving camera
                if (!bIsFiring) iSyncRate = 1000;

                if ((dwThisTick - m_dwLastSendAimSyncTick) > iSyncRate)
                {
                    SendAimSyncData();
                    m_dwLastSendAimSyncTick = dwThisTick;
                }
            }
            m_bPassengerDriveByMode = false;
        }
        // PASSENGER LOGIC
        else if (m_pPlayerPed->GetActionTrigger() == ACTION_INCAR && m_pPlayerPed->IsAPassenger())
        {
            MaybeSendExitVehicle();
            g_bLockEnterVehicleWidget = false;

            CVehicleGTA* pGtaVehicle = m_pPlayerPed->GetGtaVehicle();
            m_nLastVehicle = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(pGtaVehicle);
            
            // Note: Update m_CurrentVehicle for passenger too
            m_CurrentVehicle = m_nLastVehicle;

            MoveHeadWithCamera();

            if (m_bInRCMode)
            {
                m_bInRCMode = false;
                m_pPlayerPed->m_pPed->Add();
            }

            // Drive-By Trigger Logic
            if (!m_bPassengerDriveByMode && LocalPlayerKeys.bKeys[KEY_CTRL_BACK]) 
            {
                int curWep = m_pPlayerPed->GetCurrentWeapon();
                if (curWep == WEAPON_MICRO_UZI || curWep == WEAPON_MP5 || curWep == WEAPON_TEC9) 
                {
                    if (m_pPlayerPed->StartPassengerDriveByMode()) {
                        m_bPassengerDriveByMode = true;
                    }
                }
            }

            // Sync is handled by SendPassengerFullSyncData internally checking timer
             if ((dwThisTick - m_dwLastSendTick) > GetOptimumInCarSendRate())
            {
                m_dwLastSendTick = dwThisTick;
                SendPassengerFullSyncData();
            }
        }
    }

    // Post-Process Handling (Spectate & Respawn)
    
    // Handle Spectator waiting for spawn
    if (m_bIsSpectating && !m_bIsActive)
    {
        if (m_bSpawnDialogShowed)
        {
            m_bSpawnDialogShowed = false;
            if(pUI) pUI->spawn()->setVisible(false);
        }
        ProcessSpectating();
        return true;
    }

    // Handle Respawn Logic
    if (m_bIsWasted && m_pPlayerPed->GetActionTrigger() != ACTION_WASTED && m_pPlayerPed->GetActionTrigger() != ACTION_DEATH)
    {
        m_pPlayerPed->FlushAttach();

        if (IsClearedToSpawn() && pNetGame->GetGameState() == GAMESTATE_CONNECTED)
        {
            if (m_pPlayerPed->GetHealth() > 0.0f) {
                Spawn();
            }
        }
        else
        {
            m_bIsWasted = false;
            HandleClassSelection();
        }
        return true;
    }

    // Class Selection
    if (m_pPlayerPed->GetActionTrigger() != ACTION_WASTED &&
        m_pPlayerPed->GetActionTrigger() != ACTION_DEATH &&
        pNetGame->GetGameState() == GAMESTATE_CONNECTED &&
        !m_bIsActive &&
        !m_bIsSpectating)
    {
        ProcessClassSelection();
    }

    // Final Aim Sync Check (Safety Net)
    uint16_t wKeys, lrAnalog, udAnalog;
    wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog, false);

    if ((dwThisTick - m_dwLastAimSendTick) > CalculateAimSendRate(wKeys)) {
        m_dwLastAimSendTick = dwThisTick;
        SendAimSyncData();
    }

    return true;
}

/*void CLocalPlayer::SendDeath()
{
    RakNet::BitStream bsPlayerDeath;

    bsPlayerDeath.Write((uint16)	pNetGame->GetPlayerPool()->GetLocalPlayer()->lastDamageId);
    bsPlayerDeath.Write((uint8)		pNetGame->GetPlayerPool()->GetLocalPlayer()->lastDamageWeap);

    pNetGame->GetRakClient()->RPC(&RPC_Death, &bsPlayerDeath, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}*/

void CLocalPlayer::GoEnterVehicle(bool passenger)
{
	if (CTimer::m_snTimeInMillisecondsNonClipped - m_dwPassengerEnterExit < 1000)
		return;

    CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
    if (!pVehiclePool) return;

	VEHICLEID ClosetVehicleID = pVehiclePool->FindNearestToLocalPlayerPed();
	if (ClosetVehicleID != INVALID_VEHICLE_ID)
	{
		CVehicle* pVehicle = pVehiclePool->GetAt(ClosetVehicleID);

		if (pVehicle != nullptr && pVehicle->m_pVehicle->GetDistanceFromLocalPlayerPed() < 4.0f)
		{
			m_pPlayerPed->EnterVehicle(pVehicle->m_dwGTAId, passenger);
			SendEnterVehicleNotification(ClosetVehicleID, passenger);
			m_dwPassengerEnterExit = CTimer::m_snTimeInMillisecondsNonClipped;
		}
	}
}

void CLocalPlayer::ProcessClassSelection()
{
    if (!m_bSpawnDialogShowed)
    {
        if (pUI) pUI->spawn()->setVisible(true);
        RequestClass(m_iSelectedClass);
        m_bSpawnDialogShowed = true;
    }
}

void CLocalPlayer::ToggleSpectating(bool bToggle)
{
    if (m_bIsSpectating && !bToggle) {
        Spawn();
    }

    m_bIsSpectating = bToggle;
    m_byteSpectateType = SPECTATE_TYPE_NONE;
    m_bSpectateProcessed = false;
    m_SpectateID = 0xFFFFFFFF;
}

void CLocalPlayer::ProcessSpectating()
{
    CCamera& TheCamera = *reinterpret_cast<CCamera*>(g_libGTASA + (VER_x32 ? 0x00951FA8 : 0xBBA8D0));
    
    CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
    CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();

    if (!pPlayerPool || !pVehiclePool) return;

    RakNet::BitStream bsSpectatorSync;
    SPECTATOR_SYNC_DATA spSync;
    RwMatrix matPos;

    uint16_t lrAnalog, udAnalog;
    uint16_t wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);
    
    matPos = TheCamera.GetMatrix().ToRwMatrix();

    spSync.vecPos.x = matPos.pos.x;
    spSync.vecPos.y = matPos.pos.y;
    spSync.vecPos.z = matPos.pos.z;
    spSync.lrAnalog = lrAnalog;
    spSync.udAnalog = udAnalog;
    spSync.wKeys = wKeys;

    if ((CTimer::m_snTimeInMillisecondsNonClipped - m_dwLastSendSpecTick) > GetOptimumOnFootSendRate())
    {
        m_dwLastSendSpecTick = CTimer::m_snTimeInMillisecondsNonClipped;
        bsSpectatorSync.Write((uint8_t)ID_SPECTATOR_SYNC);
        bsSpectatorSync.Write((char*)&spSync, sizeof(SPECTATOR_SYNC_DATA));
        pNetGame->GetRakClient()->Send(&bsSpectatorSync, HIGH_PRIORITY, UNRELIABLE, 0);

        if ((CTimer::m_snTimeInMillisecondsNonClipped - m_dwLastSendAimSyncTick) > (GetOptimumOnFootSendRate() * 2))
        {
            m_dwLastSendAimSyncTick = CTimer::m_snTimeInMillisecondsNonClipped;
            SendAimSyncData();
        }
    }

    pGame->DisplayHUD(false);
    m_pPlayerPed->SetHealth(100.0f);
    m_pPlayerPed->m_pPed->SetPosn(spSync.vecPos.x, spSync.vecPos.y, spSync.vecPos.z + 20.0f);

    if (m_byteSpectateType == SPECTATE_TYPE_PLAYER)
    {
        if (!pPlayerPool->GetSlotState(m_SpectateID) || 
            !pPlayerPool->GetAt(m_SpectateID)->IsActive() ||
             pPlayerPool->GetAt(m_SpectateID)->GetState() == PLAYER_STATE_WASTED)
        {
            m_byteSpectateType = SPECTATE_TYPE_NONE;
            m_bSpectateProcessed = false;
        }
    }

    if (m_bSpectateProcessed) return;

    if (m_byteSpectateType == SPECTATE_TYPE_NONE)
    {
        GetPlayerPed()->RemoveFromVehicleAndPutAt(0.0f, 0.0f, 10.0f);
        CCamera::SetPosition(50.0f, 50.0f, 50.0f, 0.0f, 0.0f, 0.0f);
        CCamera::LookAtPoint(60.0f, 60.0f, 50.0f, 2);
        m_bSpectateProcessed = true;
    }
    else if (m_byteSpectateType == SPECTATE_TYPE_PLAYER)
    {
        if (pPlayerPool->GetSlotState(m_SpectateID))
        {
            CPlayerPed* pTargetPed = pPlayerPool->GetAt(m_SpectateID)->GetPlayerPed();
            if (pTargetPed)
            {
                TheCamera.TakeControl(pTargetPed->m_pPed, static_cast<eCamMode>(m_byteSpectateMode), eSwitchType::JUMPCUT, 1);
                m_bSpectateProcessed = true;
            }
        }
    }
    else if (m_byteSpectateType == SPECTATE_TYPE_VEHICLE)
    {
        CVehicle* pVehicle = pVehiclePool->GetAt((VEHICLEID)m_SpectateID);
        if (pVehicle)
        {
            TheCamera.TakeControl(pVehicle->m_pVehicle, static_cast<eCamMode>(m_byteSpectateMode), eSwitchType::JUMPCUT, 1);
            m_bSpectateProcessed = true;
        }
    }
}

bool CLocalPlayer::Spawn()
{
    if (!m_bHasSpawnInfo) {
        return false;
    }

    // voice
    SpeakerList::Show();
    MicroIcon::Show();

    if (m_bSpawnDialogShowed)
    {
        m_bSpawnDialogShowed = false;
        if (pUI) pUI->spawn()->setVisible(false);
    }

    CCamera& TheCamera = *reinterpret_cast<CCamera*>(g_libGTASA + (VER_x32 ? 0x00951FA8 : 0xBBA8D0));
    TheCamera.RestoreWithJumpCut();
    CCamera::SetBehindPlayer();

    pGame->DisplayHUD(true);
    m_pPlayerPed->TogglePlayerControllable(true);

    if (!bFirstSpawn) {
        m_pPlayerPed->SetInitialState();
    }
    else {
        bFirstSpawn = false;
    }

    pGame->RefreshStreamingAt(m_SpawnInfo.vecPos.x, m_SpawnInfo.vecPos.y);
    pGame->DisableTrainTraffic();

    if (m_pPlayerPed->IsCuffed()) {
        // m_pPlayerPed->ToggleCuffed(false);
    }

    m_pPlayerPed->RestartIfWastedAt(&m_SpawnInfo.vecPos, m_SpawnInfo.fRotation);
    m_pPlayerPed->SetModelIndex(m_SpawnInfo.iSkin);

    m_pPlayerPed->ClearAllWeapons();
    m_pPlayerPed->ResetDamageEntity();

    if (m_SpawnInfo.iSpawnWeapons[2] != -1) {
        m_pPlayerPed->GiveWeapon(m_SpawnInfo.iSpawnWeapons[2], m_SpawnInfo.iSpawnWeaponsAmmo[2]);
    }
    if (m_SpawnInfo.iSpawnWeapons[1] != -1) {
        m_pPlayerPed->GiveWeapon(m_SpawnInfo.iSpawnWeapons[1], m_SpawnInfo.iSpawnWeaponsAmmo[1]);
    }
    if (m_SpawnInfo.iSpawnWeapons[0] != -1) {
        m_pPlayerPed->GiveWeapon(m_SpawnInfo.iSpawnWeapons[0], m_SpawnInfo.iSpawnWeaponsAmmo[0]);
    }

    m_pPlayerPed->m_pPed->SetPosn(m_SpawnInfo.vecPos.x, m_SpawnInfo.vecPos.y, m_SpawnInfo.vecPos.z + 0.5f);

    m_pPlayerPed->ForceTargetRotation(m_SpawnInfo.fRotation);

    m_bIsWasted = false;
    m_bDeathSended = false;
    m_bIsActive = true;
    m_bWaitingForSpawnRequestReply = false;
    m_surfData.bIsActive = false;

    RakNet::BitStream bsSendSpawn;
    pNetGame->GetRakClient()->RPC(&RPC_Spawn, &bsSendSpawn, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);

    m_iDisplayZoneTick = CTimer::m_snTimeInMillisecondsNonClipped + 1000;

    return true;
}

void CLocalPlayer::HandleClassSelection()
{
    m_bClearedToSpawn = false;

    if(m_pPlayerPed)
    {
        m_pPlayerPed->SetInitialState();
        m_pPlayerPed->SetHealth(100.0f);
        m_pPlayerPed->TogglePlayerControllable(false);
    }

    RequestClass(m_iSelectedClass);
}

void CLocalPlayer::SendWastedNotification()
{
    uint8_t byteDeathReason;
    PLAYERID WhoWasResponsible = INVALID_PLAYER_ID;
    RakNet::BitStream bsPlayerDeath;
    byteDeathReason = m_pPlayerPed->FindDeathReasonAndResponsiblePlayer(&WhoWasResponsible);
    bsPlayerDeath.Write(byteDeathReason);
    bsPlayerDeath.Write(WhoWasResponsible);
    FLog("SendWastedNotification %d %d", byteDeathReason, WhoWasResponsible);
    pNetGame->GetRakClient()->RPC(&RPC_Death, &bsPlayerDeath, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::SendOnFootFullSyncData()
{
    RakNet::BitStream bsPlayerSync;
    RwMatrix matPlayer;
    CVector vecMoveSpeed;
    uint16_t lrAnalog, udAnalog;
    uint8_t exKeys = m_pPlayerPed->GetAdditionalKeys();
    uint16_t wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);

    ONFOOT_SYNC_DATA ofSync;

    matPlayer = m_pPlayerPed->m_pPed->GetMatrix().ToRwMatrix();
    vecMoveSpeed = m_pPlayerPed->m_pPed->GetMoveSpeed();

    ofSync.lrAnalog = lrAnalog;
    ofSync.udAnalog = udAnalog;
    ofSync.wKeys = wKeys;
    ofSync.vecPos.x = matPlayer.pos.x;
    ofSync.vecPos.y = matPlayer.pos.y;
    ofSync.vecPos.z = matPlayer.pos.z;

    ofSync.quat.SetFromMatrix(&matPlayer);
    ofSync.quat.Normalize();

    if (FloatOffset(ofSync.quat.w, m_LastSendOnFootSync.quat.w) < 0.00001 &&
        FloatOffset(ofSync.quat.x, m_LastSendOnFootSync.quat.x) < 0.00001 &&
        FloatOffset(ofSync.quat.y, m_LastSendOnFootSync.quat.y) < 0.00001 &&
        FloatOffset(ofSync.quat.z, m_LastSendOnFootSync.quat.z) < 0.00001)
    {
        ofSync.quat.Set(m_LastSendOnFootSync.quat);
    }

    ofSync.byteHealth = (uint8_t)m_pPlayerPed->GetHealth();
    ofSync.byteArmour = (uint8_t)m_pPlayerPed->GetArmour();

    ofSync.byteCurrentWeapon = (exKeys << 6) | ofSync.byteCurrentWeapon & 0x3F;
    ofSync.byteCurrentWeapon ^= (ofSync.byteCurrentWeapon ^ m_pPlayerPed->GetCurrentWeapon()) & 0x3F;
    ofSync.byteSpecialAction = GetSpecialAction();
    ofSync.vecMoveSpeed.x = vecMoveSpeed.x;
    ofSync.vecMoveSpeed.y = vecMoveSpeed.y;
    ofSync.vecMoveSpeed.z = vecMoveSpeed.z;

    ofSync.vecSurfOffsets.x = 0.0f;
    ofSync.vecSurfOffsets.y = 0.0f;
    ofSync.vecSurfOffsets.z = 0.0f;
    ofSync.wSurfInfo = 0;
    if(m_surfData.bIsActive){
        if(m_surfData.bIsVehicle && m_surfData.dwSurfVehID != INVALID_VEHICLE_ID){
            CVehicle* pVeh = (CVehicle*)m_surfData.pSurfInst;
            ofSync.vecSurfOffsets.x = m_surfData.vecOffsetPos.x;
            ofSync.vecSurfOffsets.y = m_surfData.vecOffsetPos.y;
            ofSync.vecSurfOffsets.z = m_surfData.vecOffsetPos.z;
            ofSync.wSurfInfo = m_surfData.dwSurfVehID;
        }
    }

    ofSync.dwAnimation = 0;

    if ((CTimer::m_snTimeInMillisecondsNonClipped - m_dwLastUpdateOnFootData) > 500 || memcmp(&m_LastSendOnFootSync, &ofSync, sizeof(ONFOOT_SYNC_DATA)))
    {
        m_dwLastUpdateOnFootData = CTimer::m_snTimeInMillisecondsNonClipped;

        bsPlayerSync.Write((uint8_t)ID_PLAYER_SYNC);
        bsPlayerSync.Write((char*)&ofSync, sizeof(ONFOOT_SYNC_DATA));
        pNetGame->GetRakClient()->Send(&bsPlayerSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1);
        m_LastSendOnFootSync = ofSync;
    }
}

void CLocalPlayer::SendInCarFullSyncData()
{
    if (!m_pPlayerPed || !m_pPlayerPed->m_pPed) return;
    
    INCAR_SYNC_DATA icSync;
    memset(&icSync, 0, sizeof(INCAR_SYNC_DATA));

    CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
    if (!pVehiclePool) return;

    CVehicle *pVehicle = m_pPlayerPed->GetCurrentVehicle();
    if (!pVehicle) return;

    VEHICLEID vehicleid = pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
    if (vehicleid == INVALID_VEHICLE_ID) return;

    icSync.VehicleID = vehicleid;

    uint16_t lrAnalog, udAnalog;
    uint8_t exKeys = m_pPlayerPed->GetAdditionalKeys();
    uint16_t wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);

    icSync.lrAnalog = lrAnalog;
    icSync.udAnalog = udAnalog;
    icSync.wKeys = wKeys;

    icSync.byteCurrentWeapon = (exKeys << 6) | (icSync.byteCurrentWeapon & 0x3F);
    icSync.byteCurrentWeapon ^= (icSync.byteCurrentWeapon ^ m_pPlayerPed->GetCurrentWeapon()) & 0x3F;

    RwMatrix mat;
    CVector vecMoveSpeed;
    
    pVehicle->m_pVehicle->GetMatrix(&mat);
    vecMoveSpeed = pVehicle->m_pVehicle->GetMoveSpeed();

    icSync.quat.SetFromMatrix(&mat);
    icSync.quat.Normalize();

    if (FloatOffset(icSync.quat.w, m_InCarData.quat.w) < 0.00001f &&
        FloatOffset(icSync.quat.x, m_InCarData.quat.x) < 0.00001f &&
        FloatOffset(icSync.quat.y, m_InCarData.quat.y) < 0.00001f &&
        FloatOffset(icSync.quat.z, m_InCarData.quat.z) < 0.00001f)
    {
        icSync.quat.Set(m_InCarData.quat);
    }

    icSync.vecPos = mat.pos;
    icSync.vecMoveSpeed = vecMoveSpeed;

    icSync.fCarHealth = pVehicle->GetHealth();
    icSync.bytePlayerHealth = (uint8_t)m_pPlayerPed->GetHealth();
    icSync.bytePlayerArmour = (uint8_t)m_pPlayerPed->GetArmour();

    int iModelID = pVehicle->m_pVehicle->GetModelId();
    
    if (iModelID == TRAIN_PASSENGER_LOCO || 
        iModelID == TRAIN_FREIGHT_LOCO || 
        iModelID == TRAIN_TRAM)
    {
        icSync.fTrainSpeed = pVehicle->GetTrainSpeed();
    }
    else if (pVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_BIKE || 
             pVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_PUSHBIKE)
    {
        icSync.fTrainSpeed = pVehicle->GetBikeLean();
    }
    else if (iModelID == HYDRA)
    {
        icSync.fTrainSpeed = (float)pVehicle->GetHydraThrusters();
    }
    else
    {
        icSync.fTrainSpeed = 0.0f;
    }

    icSync.byteSirenOn = pVehicle->SirenEnabled() ? 1 : 0;

    if (pVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_PLANE)
    {
        icSync.byteLandingGearState = pVehicle->IsLandingGearNotUp() ? 1 : 0;
    }

    icSync.TrailerID = 0;
    CVehicle *pTrailer = pVehicle->GetTrailer();

    if (pTrailer && pTrailer->m_pVehicle)
    {
        if (pTrailer->GetTractor() == pVehicle)
        {
             pVehicle->SetTrailer(pTrailer);
             icSync.TrailerID = pVehiclePool->FindIDFromGtaPtr(pTrailer->m_pVehicle);
             
             if (icSync.TrailerID != INVALID_VEHICLE_ID && icSync.TrailerID < MAX_VEHICLES)
             {
                TRAILER_SYNC_DATA trSync;
                RwMatrix matTrailer;
                
                pTrailer->m_pVehicle->GetMatrix(&matTrailer);
                
                trSync.trailerID = icSync.TrailerID;
                trSync.vecPos = matTrailer.pos;
                
                CQuaternion syncQuat;
                syncQuat.SetFromMatrix(&matTrailer);
                trSync.quat = syncQuat;
                
                trSync.vecMoveSpeed = pTrailer->m_pVehicle->GetMoveSpeed();
                trSync.vecTurnSpeed = pTrailer->m_pVehicle->GetTurnSpeed(); // TurnSpeed สำคัญสำหรับรถพ่วง

                RakNet::BitStream bsTrailerSync;
                bsTrailerSync.Write((uint8_t)ID_TRAILER_SYNC);
                bsTrailerSync.Write((char*)&trSync, sizeof(TRAILER_SYNC_DATA));
                pNetGame->GetRakClient()->Send(&bsTrailerSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
             }
        }
    }
    else 
    {
        pVehicle->SetTrailer(NULL);
    }

    if ((CTimer::m_snTimeInMillisecondsNonClipped - m_dwLastUpdateInCarData) > 500 || 
        memcmp(&m_InCarData, &icSync, sizeof(INCAR_SYNC_DATA)))
    {
        RakNet::BitStream bsVehicleSync;
        bsVehicleSync.Write((uint8_t)ID_VEHICLE_SYNC);
        bsVehicleSync.Write((char *)&icSync, sizeof(INCAR_SYNC_DATA));
        pNetGame->GetRakClient()->Send(&bsVehicleSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

        memcpy(&m_InCarData, &icSync, sizeof(INCAR_SYNC_DATA));
        m_dwLastUpdateInCarData = CTimer::m_snTimeInMillisecondsNonClipped;
    }
}

void CLocalPlayer::SendTrailerData(VEHICLEID vehicleId)
{
    TRAILER_SYNC_DATA trSync;
    memset(&trSync, 0, sizeof(TRAILER_SYNC_DATA));

    CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
    if(!pVehiclePool) return;

    CVehicle* pTrailer = pVehiclePool->GetAt(vehicleId);
    if(pTrailer)
    {
        RwMatrix matTrailer = pTrailer->m_pVehicle->GetMatrix().ToRwMatrix();
        CQuaternion syncQuat;
        syncQuat.SetFromMatrix(&matTrailer);
        trSync.quat = syncQuat;

        trSync.trailerID = vehicleId;

        trSync.vecPos.x = matTrailer.pos.x;
        trSync.vecPos.y = matTrailer.pos.y;
        trSync.vecPos.z = matTrailer.pos.z;

        trSync.vecMoveSpeed = pTrailer->m_pVehicle->GetMoveSpeed();
        trSync.vecTurnSpeed = pTrailer->m_pVehicle->GetTurnSpeed();

        //if(IsNeedSyncDataSend(&m_TrailerData, &trSync, sizeof(TRAILER_SYNC_DATA)))
        //{
        RakNet::BitStream bsTrailerSync;
        bsTrailerSync.Write((uint8_t)ID_TRAILER_SYNC);
        bsTrailerSync.Write((char*)&trSync, sizeof (TRAILER_SYNC_DATA));
        pNetGame->GetRakClient()->Send(&bsTrailerSync,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,0);

        memcpy(&m_TrailerData, &trSync, sizeof(TRAILER_SYNC_DATA));
        //}
    }
}

void CLocalPlayer::SendPassengerFullSyncData()
{
    if (!m_pPlayerPed || !m_pPlayerPed->m_pPed) return;

    RakNet::BitStream bsData;
    CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
    if (!pVehiclePool) return;

    uint16_t lrAnalog, udAnalog;
    uint8_t exKeys = m_pPlayerPed->GetAdditionalKeys();
    uint16_t wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);

    PASSENGER_SYNC_DATA psSync;
    memset(&psSync, 0, sizeof(PASSENGER_SYNC_DATA));

    CVehicleGTA* pGtaVehicle = m_pPlayerPed->GetGtaVehicle();
    psSync.VehicleID = pVehiclePool->FindIDFromGtaPtr(pGtaVehicle);
    
    if (psSync.VehicleID == INVALID_VEHICLE_ID) return;

    psSync.lrAnalog = lrAnalog;
    psSync.udAnalog = udAnalog;
    psSync.wKeys = wKeys;
    
    psSync.byteCurrentWeapon = (exKeys << 6) | (psSync.byteCurrentWeapon & 0x3F);
    psSync.byteCurrentWeapon ^= (psSync.byteCurrentWeapon ^ m_pPlayerPed->GetCurrentWeapon()) & 0x3F;

    psSync.bytePlayerHealth = (uint8_t)m_pPlayerPed->GetHealth();
    psSync.bytePlayerArmour = (uint8_t)m_pPlayerPed->GetArmour();

    psSync.byteSeatFlags = m_pPlayerPed->GetVehicleSeatID(); 
    
    uint8_t byteUnk = psSync.byteSeatFlags & 0x7F;
    psSync.byteSeatFlags = (byteUnk ^ (m_pPlayerPed->IsInPassengerDriveByMode() << 6)) & 0x40 ^ byteUnk;

    psSync.vecPos = m_pPlayerPed->m_pPed->GetPosition();

    if ((CTimer::m_snTimeInMillisecondsNonClipped - m_dwLastUpdatePassengerData) > 500 || 
        memcmp(&m_PassengerData, &psSync, sizeof(PASSENGER_SYNC_DATA)))
    {
        m_dwLastUpdatePassengerData = CTimer::m_snTimeInMillisecondsNonClipped;

        bsData.Write((uint8_t)ID_PASSENGER_SYNC);
        bsData.Write((char*)&psSync, sizeof(PASSENGER_SYNC_DATA));
        pNetGame->GetRakClient()->Send(&bsData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

        memcpy(&m_PassengerData, &psSync, sizeof(PASSENGER_SYNC_DATA));
    }

    if(m_bPassengerDriveByMode) 
    {
        SendAimSyncData();
    }
}

void CLocalPlayer::SendAimSyncData()
{
    if (!m_pPlayerPed) return;

    AIM_SYNC_DATA aimSync;
    memset(&aimSync, 0, sizeof(AIM_SYNC_DATA));

    CAMERA_AIM* caAim = m_pPlayerPed->GetCurrentAim();
    if (!caAim) return;

    aimSync.byteCamMode = m_pPlayerPed->GetCameraMode();

    aimSync.vecAimf.x = caAim->f1x;
    aimSync.vecAimf.y = caAim->f1y;
    aimSync.vecAimf.z = caAim->f1z;

    aimSync.vecAimPos.x = caAim->pos1x;
    aimSync.vecAimPos.y = caAim->pos1y;
    aimSync.vecAimPos.z = caAim->pos1z;

    aimSync.fAimZ = m_pPlayerPed->GetAimZ();

    float fAspectRatio = GameGetAspectRatio() * 255.0f;
    if (fAspectRatio > 255.0f) fAspectRatio = 255.0f;
    aimSync.aspect_ratio = (uint8_t)fAspectRatio;

    aimSync.byteCamExtZoom = static_cast<unsigned char>(m_pPlayerPed->GetCameraExtendedZoom() * 63.0f) & 63;

    CWeapon* pwstWeapon = m_pPlayerPed->GetCurrentWeaponSlot();
    if (pwstWeapon)
    {
        if (pwstWeapon->dwState == 2)
            aimSync.byteWeaponState = WEAPONSTATE_RELOADING;
        else
            aimSync.byteWeaponState = (pwstWeapon->dwAmmoInClip > 1) ? WEAPONSTATE_FIRING : (uint8_t)pwstWeapon->dwAmmoInClip;
    }

    if ((CTimer::m_snTimeInMillisecondsNonClipped - m_dwLastAimSendTick) > 500 || 
        memcmp(&m_aimSync, &aimSync, sizeof(AIM_SYNC_DATA)))
    {
        m_dwLastAimSendTick = CTimer::m_snTimeInMillisecondsNonClipped;

        RakNet::BitStream bsAimSync;
        bsAimSync.Write((uint8_t)ID_AIM_SYNC);
        bsAimSync.Write((char*)&aimSync, sizeof(AIM_SYNC_DATA));
        
        pNetGame->GetRakClient()->Send(&bsAimSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1);

        memcpy(&m_aimSync, &aimSync, sizeof(AIM_SYNC_DATA));
    }
}

void CLocalPlayer::SendStatsUpdate()
{
    if(m_statsData.dwLastMoney != pGame->GetLocalMoney() ||
       m_statsData.dwLastDrunkLevel != m_pPlayerPed->GetDrunkLevel())
    {
        m_statsData.dwLastMoney = pGame->GetLocalMoney();
        m_statsData.dwLastDrunkLevel = m_pPlayerPed->GetDrunkLevel();

        RakNet::BitStream bsStats;
        bsStats.Write((uint8_t)ID_STATS_UPDATE);
        bsStats.Write(m_statsData.dwLastMoney);
        bsStats.Write(m_statsData.dwLastDrunkLevel);
        pNetGame->GetRakClient()->Send(&bsStats, HIGH_PRIORITY, UNRELIABLE, 0);
    }
}

void CLocalPlayer::CheckWeapons()
{
    if (m_pPlayerPed->IsInVehicle()) return;

	RakNet::BitStream bs;
	bs.Write((uint8_t) ID_WEAPONS_UPDATE);

	bs.Write((uint16_t)INVALID_PLAYER_ID);
	bs.Write((uint16_t)INVALID_PLAYER_ID);

    bool bMSend = false;

    for (int i = 0; i < 13; i++) {

        if (m_byteLastWeapon[i] != m_pPlayerPed->m_pPed->m_aWeapons[i].dwType ||
            m_dwLastAmmo[i] != m_pPlayerPed->m_pPed->m_aWeapons[i].dwAmmo)
        {
            m_byteLastWeapon[i] = m_pPlayerPed->m_pPed->m_aWeapons[i].dwType;
            m_dwLastAmmo[i] = m_pPlayerPed->m_pPed->m_aWeapons[i].dwAmmo;

            bMSend = true;
            break;
        }

    }

    if (bMSend) {
        for (int i = 0; i < 13; i++) {
            bs.Write((uint8_t) i);
            bs.Write((uint8_t) m_byteLastWeapon[i]);
            bs.Write((uint16_t) m_dwLastAmmo[i]);
        }
        pNetGame->GetRakClient()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
    }
}

uint32_t CLocalPlayer::CalculateAimSendRate(uint16_t wKeys) {
	uint32_t baseRate = NETMODE_HEADSYNC_SENDRATE;

	if (wKeys & 128) {
        if (wKeys & 4) {
			baseRate = NETMODE_FIRING_SENDRATE + m_nPlayersInRange;
		} else {
			baseRate = NETMODE_AIM_SENDRATE + m_nPlayersInRange;
		}
	} else if (wKeys & 4) {
		baseRate = GetOptimumOnFootSendRate();
	}

	return static_cast<uint32_t>(baseRate);
}

void CLocalPlayer::UpdateRemoteInterior(uint8_t byteInterior)
{
    m_byteCurInterior = byteInterior;
    RakNet::BitStream bsUpdateInterior;
    bsUpdateInterior.Write(byteInterior);
    pNetGame->GetRakClient()->RPC(&RPC_SetInteriorId, &bsUpdateInterior, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::UpdateCameraTarget()
{

}

void CLocalPlayer::ProcessSurfing() {
    if(m_pPlayerPed && !m_pPlayerPed->IsDead() && !LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP]) {
        CVehicleGTA* contactVeh = (CVehicleGTA*)m_pPlayerPed->GetEntityUnderPlayer();
        if(contactVeh){
            VEHICLEID vehicleId = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(contactVeh);
            if(vehicleId && vehicleId != INVALID_VEHICLE_ID){
                CVehicle* pVeh = pNetGame->GetVehiclePool()->GetAt(vehicleId);
                if(pVeh && (pVeh->HasADriver() || pVeh->m_pVehicle->GetModelId() == 569 || pVeh->m_pVehicle->GetModelId() == 570)
                   && pVeh->m_pVehicle->GetDistanceFromLocalPlayerPed() < 30.0){
                    /*bool onFootObject = ScriptCommand(&is_char_touching_vehicle, m_pPlayerPed->m_dwGTAId, pVeh->m_dwGTAId);
                    if(onFootObject){*/
                    if(m_surfData.bIsActive){
                        return;
                    }
                    memset(&m_surfData, 0, sizeof(m_surfData));
                    m_surfData.vecOffsetPos = CVector{0.0f, 0.0f, 0.0f};
                    m_surfData.dwSurfVehID = vehicleId;
                    m_surfData.pSurfInst = (uintptr_t)pVeh;
                    m_surfData.bIsVehicle = true;

                    static RwMatrix matVeh;
                    matVeh = pVeh->m_pVehicle->GetMatrix().ToRwMatrix();
                    static RwMatrix matPed;
                    matPed = m_pPlayerPed->m_pPed->GetMatrix().ToRwMatrix();
                    static RwMatrix matOut;
                    mat_invert(&matOut, &matVeh);
                    ProjectMatrix(&m_surfData.vecOffsetPos, (CMatrix*)(&matOut), (CVector*)&matPed.pos);

                    m_surfData.bIsActive = true;
                    return;
                    //}
                }
            }else{
                CPhysical* contactEntity = (CPhysical*)m_pPlayerPed->GetEntityUnderPlayer();
                if(contactEntity){
                    uint32_t objectId = pNetGame->GetObjectPool()->FindIDFromGtaPtr(
                            (contactEntity));
                    if(objectId && objectId != INVALID_OBJECT_ID){
                        CObject* pObject = pNetGame->GetObjectPool()->GetAt(objectId);
                        if(pObject){
                            //bool onFootObject = ScriptCommand(&is_char_touching_object, m_pPlayerPed->m_dwGTAId, pObject->m_dwGTAId);
                            //if(onFootObject) {
                            if(m_surfData.bIsActive){
                                return;
                            }
                            memset(&m_surfData, 0, sizeof(m_surfData));
                            m_surfData.bIsVehicle = false;
                            m_surfData.pSurfInst = (uintptr_t) pObject;
                            m_surfData.bIsActive = true;
                            return;
                            //}
                        }
                    }
                }
            }
        }
    }

    m_surfData.bIsActive = false;
    m_surfData.dwSurfVehID = INVALID_VEHICLE_ID;
    m_surfData.pSurfInst = 0;
    m_surfData.vecOffsetPos = CVector{0.0f, 0.0f, 0.0f};

}
void CLocalPlayer::UpdateSurfing() {
    static RwMatrix surfInstMatrix;
    static RwMatrix surfPedMatrix;
    static CVector surfInstMoveSpeed;
    static CVector surfInstTurnSpeed;
    if(m_pPlayerPed) {
        if (LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP]) {
            return;
        }
        if (m_surfData.bIsActive) {
            if (m_surfData.bIsVehicle && m_surfData.pSurfInst) {
                CVehicle *pVeh = (CVehicle *) m_surfData.pSurfInst;
                surfInstMatrix = pVeh->m_pVehicle->GetMatrix().ToRwMatrix();
                surfPedMatrix = m_pPlayerPed->m_pPed->GetMatrix().ToRwMatrix();
                surfInstMoveSpeed = pVeh->m_pVehicle->GetMoveSpeed();
                surfInstTurnSpeed = pVeh->m_pVehicle->GetTurnSpeed();

                uint16_t lrAnalog;
                uint16_t udAnalog;
                m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);

                if (lrAnalog || udAnalog) {
                    static RwMatrix matOut;
                    mat_invert(&matOut, &surfInstMatrix);
                    ProjectMatrix(&m_surfData.vecOffsetPos, (CMatrix*)&matOut,  (CVector*)&surfPedMatrix.pos);
                } else {
                    ProjectMatrix( (CVector*)&surfPedMatrix.pos, (CMatrix*)&surfInstMatrix,  (CVector*)&m_surfData.vecOffsetPos);

                    m_pPlayerPed->m_pPed->SetMatrix((CMatrix&)surfPedMatrix);
                    CVector vecMoveSpeed;
                    vecMoveSpeed = m_pPlayerPed->m_pPed->GetMoveSpeed();
                    m_pPlayerPed->m_pPed->SetVelocity(
                            CVector{surfInstMoveSpeed.x, surfInstMoveSpeed.y, vecMoveSpeed.z});

                    CVector vecTurnSpeed = m_pPlayerPed->m_pPed->GetTurnSpeed();
                    m_pPlayerPed->m_pPed->SetTurnSpeed(
                            CVector{vecTurnSpeed.x, vecTurnSpeed.y, surfInstTurnSpeed.z});
                }
            } else {
                CObject *pObject = (CObject *) m_surfData.pSurfInst;
                if (pObject && pObject->m_byteMoving & 1) {
                    surfInstMatrix = pObject->m_pEntity->GetMatrix().ToRwMatrix();
                    surfPedMatrix = m_pPlayerPed->m_pPed->GetMatrix().ToRwMatrix();
                    surfInstMoveSpeed = pObject->m_pEntity->GetMoveSpeed();
                    surfInstTurnSpeed = pObject->m_pEntity->GetTurnSpeed();

                    uint16_t lrAnalog;
                    uint16_t udAnalog;
                    m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);

                    if (lrAnalog || udAnalog) {
                        static RwMatrix matOut;
                        mat_invert(&matOut, &surfInstMatrix);
                        ProjectMatrix(&m_surfData.vecOffsetPos, (CMatrix*)&matOut,  (CVector*)&surfPedMatrix.pos);
                    } else {
                        ProjectMatrix( (CVector*)&surfPedMatrix.pos, (CMatrix*)&surfInstMatrix,
                                       &m_surfData.vecOffsetPos);

                        m_pPlayerPed->m_pPed->SetMatrix((CMatrix&)surfPedMatrix);
                        CVector vecMoveSpeed = m_pPlayerPed->m_pPed->GetMoveSpeed();
                        m_pPlayerPed->m_pPed->SetVelocity(
                                CVector{surfInstMoveSpeed.x, surfInstMoveSpeed.y, vecMoveSpeed.z});

                        CVector vecTurnSpeed = m_pPlayerPed->m_pPed->GetTurnSpeed();
                        m_pPlayerPed->m_pPed->SetTurnSpeed(
                                CVector{vecTurnSpeed.x, vecTurnSpeed.y, surfInstTurnSpeed.z});
                    }
                }

            }
        }

    }
}

void CLocalPlayer::MoveHeadWithCamera()
{

}

bool bNeedEnterVehicle = false;

bool CLocalPlayer::HandlePassengerEntry()
{
    CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();

    if (!bNeedEnterVehicle) return false;
    VEHICLEID ClosetVehicleID = pVehiclePool->FindNearestToLocalPlayerPed();
    CVehicle* pVehicle = pVehiclePool->GetAt(ClosetVehicleID);
    if (!pVehicle) return false;

    if (pVehicle->m_pVehicle->GetDistanceFromLocalPlayerPed() < 8.0f)
    {
        if (m_pPlayerPed->GetCurrentWeapon() == WEAPON_PARACHUTE) {
            m_pPlayerPed->SetArmedWeapon(0, false);
        }

        m_pPlayerPed->EnterVehicle(pVehicle->m_dwGTAId, true);
        SendEnterVehicleNotification(ClosetVehicleID, true);
        bNeedEnterVehicle = false;
    }
    bNeedEnterVehicle = false;
    return true;
}

void CLocalPlayer::ProcessOnFootWorldBounds()
{
    if(pGame->GetActiveInterior() != 0) return; // can't enforce inside interior

    /*if(m_pPlayerPed->EnforceWorldBoundries(
            pNetGame->m_pNetSet->fWorldBounds[0], pNetGame->m_pNetSet->fWorldBounds[1],
            pNetGame->m_pNetSet->fWorldBounds[2], pNetGame->m_pNetSet->fWorldBounds[3]))
    {
        m_pPlayerPed->SetArmedWeapon(0, 0);
        pGame->DisplayGameText("Stay within the ~r~world boundries", 1000, 5);
    }*/
}

void CLocalPlayer::ProcessInCarWorldBounds()
{
    /*CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
    if(pVehiclePool)
    {
        if(pGame->GetActiveInterior() != 0) return; // can't enforce inside interior

        VEHICLEID vehicleId = pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
        if(vehicleId != INVALID_VEHICLE_ID)
        {
            CVehicle *pVehicle = pVehiclePool->GetAt(vehicleId);
            if(pVehicle)
            {
                if(pVehicle->EnforceWorldBoundries(
                        pNetGame->m_pNetSet->fWorldBounds[0], pNetGame->m_pNetSet->fWorldBounds[1],
                        pNetGame->m_pNetSet->fWorldBounds[2], pNetGame->m_pNetSet->fWorldBounds[3]))
                {
                    pGame->DisplayGameText("Stay within the ~r~world boundries", 1000, 5);
                }
            }
        }
    }*/
}

bool CLocalPlayer::CompareOnFootSyncKeys(uint16_t wKeys, uint16_t lrAnalog, uint16_t udAnalog)
{
    return wKeys != m_LastSendOnFootSync.wKeys || udAnalog != m_LastSendOnFootSync.udAnalog || lrAnalog != m_LastSendOnFootSync.lrAnalog;
}

int CLocalPlayer::GetOptimumOnFootSendRate()
{
    if(!m_pPlayerPed) return 1000;

    if(pNetGame->m_bLanMode) return 15;
    else
    {
        int iNumPlayersInRange = 0;
        for(int i = 2; i < 120; i++)
            if(bUsedPlayerSlots[i]) iNumPlayersInRange++;

        return (iNetModeNormalOnFootSendRate + iNumPlayersInRange);
    }
}

int CLocalPlayer::GetOptimumInCarSendRate()
{
    if(!m_pPlayerPed) return 1000;

    if(pNetGame->m_bLanMode) return 15;
    else
    {
        int iNumPlayersInRange = 0;
        for(int i = 0; i < 120; i++)
            if(bUsedPlayerSlots[i]) iNumPlayersInRange++;

        return (iNetModeNormalInCarSendRate + iNumPlayersInRange);
    }
}

void CLocalPlayer::UpdateVehicleDamage(VEHICLEID vehicleID)
{

}

void CLocalPlayer::SendNextClass()
{
    RwMatrix matPlayer;

    if (!m_bSpawnDialogShowed) return;

    m_bClearedToSpawn = false;
    matPlayer = m_pPlayerPed->m_pPed->GetMatrix().ToRwMatrix();

    if (m_iSelectedClass == (pNetGame->m_pNetSet->iSpawnsAvailable - 1)) m_iSelectedClass = 0;
    else m_iSelectedClass++;

    pGame->PlaySound(1052, matPlayer.pos.x, matPlayer.pos.y, matPlayer.pos.z);
    RequestClass(m_iSelectedClass);
}

void CLocalPlayer::SendPrevClass()
{
    RwMatrix matPlayer;

    if (!m_bSpawnDialogShowed) return;

    m_bClearedToSpawn = false;
    matPlayer = m_pPlayerPed->m_pPed->GetMatrix().ToRwMatrix();

    if (m_iSelectedClass == 0) m_iSelectedClass = (pNetGame->m_pNetSet->iSpawnsAvailable - 1);
    else m_iSelectedClass--;

    pGame->PlaySound(1053, matPlayer.pos.x, matPlayer.pos.y, matPlayer.pos.z);
    RequestClass(m_iSelectedClass);
}

void CLocalPlayer::SendSpawn()
{
    if (!m_bSpawnDialogShowed) return;

    RequestSpawn();
    m_bWaitingForSpawnRequestReply = true;
}

void CLocalPlayer::RequestClass(int iClass)
{
    RakNet::BitStream bsClassRequest;
    bsClassRequest.Write(iClass);
    pNetGame->GetRakClient()->RPC(&RPC_RequestClass, &bsClassRequest, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::RequestSpawn()
{
    RakNet::BitStream bsSpawnRequest;
    pNetGame->GetRakClient()->RPC(&RPC_RequestSpawn, &bsSpawnRequest, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::ApplySpecialAction(uint8_t byteSpecialAction)
{
    if(!m_pPlayerPed) return;

    m_pPlayerPed->iSpecialAction = byteSpecialAction;

    if(byteSpecialAction != SPECIAL_ACTION_USECELLPHONE && m_pPlayerPed->IsCellphoneEnabled())
        m_pPlayerPed->ToggleCellphone(0);
    if(byteSpecialAction != SPECIAL_ACTION_USEJETPACK && m_pPlayerPed->IsInJetpackMode())
        m_pPlayerPed->StopJetpack();
    if(byteSpecialAction != SPECIAL_ACTION_HANDSUP && m_pPlayerPed->HasHandsUp())
        m_pPlayerPed->TogglePlayerControllable(true);
    if(m_pPlayerPed->GetDanceStyle() != -1)
    {
        if((byteSpecialAction != SPECIAL_ACTION_DANCE1 ||
            byteSpecialAction != SPECIAL_ACTION_DANCE2 ||
            byteSpecialAction != SPECIAL_ACTION_DANCE3 ||
            byteSpecialAction != SPECIAL_ACTION_DANCE4))
        {
            m_pPlayerPed->StopDancing();
        }
    }
    if(byteSpecialAction != SPECIAL_ACTION_PISSING && m_pPlayerPed->IsPissing())
        m_pPlayerPed->StopPissing();
    if(m_pPlayerPed->GetStuff() != eStuffType::STUFF_TYPE_NONE)
    {
        if(byteSpecialAction != SPECIAL_ACTION_DRINK_BEER ||
           byteSpecialAction != SPECIAL_ACTION_SMOKE_CIGGY ||
           byteSpecialAction != SPECIAL_ACTION_DRINK_WINE ||
           byteSpecialAction != SPECIAL_ACTION_DRINK_SPRUNK)
        {
            m_pPlayerPed->DropStuff();
        }
    }

    switch(byteSpecialAction)
    {
        default:
        case SPECIAL_ACTION_NONE:
            // ~
            break;

        case SPECIAL_ACTION_USECELLPHONE:
            m_pPlayerPed->ToggleCellphone(1);
            break;

        case SPECIAL_ACTION_STOPUSECELLPHONE:
            if(m_pPlayerPed->IsCellphoneEnabled()) m_pPlayerPed->ToggleCellphone(0);
            break;

        case SPECIAL_ACTION_USEJETPACK:
            if(!m_pPlayerPed->IsInJetpackMode()) m_pPlayerPed->StartJetpack();
            break;

        case SPECIAL_ACTION_HANDSUP:
            m_pPlayerPed->HandsUp();
            break;

        case SPECIAL_ACTION_PISSING:
            m_pPlayerPed->StartPissing();
            break;

        case SPECIAL_ACTION_DANCE1:
            m_pPlayerPed->StartDancing(1);
            break;

        case SPECIAL_ACTION_DANCE2:
            m_pPlayerPed->StartDancing(2);
            break;

        case SPECIAL_ACTION_DANCE3:
            m_pPlayerPed->StartDancing(3);
            break;

        case SPECIAL_ACTION_DANCE4:
            m_pPlayerPed->StartDancing(4);
            break;

        case SPECIAL_ACTION_DRINK_BEER:
            m_pPlayerPed->GiveStuff(eStuffType::STUFF_TYPE_BEER);
            break;

        case SPECIAL_ACTION_SMOKE_CIGGY:
            m_pPlayerPed->GiveStuff(eStuffType::STUFF_TYPE_CIGGI);
            break;

        case SPECIAL_ACTION_DRINK_WINE:
            m_pPlayerPed->GiveStuff(eStuffType::STUFF_TYPE_DYN_BEER);
            break;

        case SPECIAL_ACTION_DRINK_SPRUNK:
            m_pPlayerPed->GiveStuff(eStuffType::STUFF_TYPE_PINT_GLASS);
            break;
    }
}

void CLocalPlayer::SetSpawnInfo(PLAYER_SPAWN_INFO* pSpawnInfo)
{
    memcpy(&m_SpawnInfo, pSpawnInfo, sizeof(PLAYER_SPAWN_INFO));
    m_bHasSpawnInfo = true;
}

void CLocalPlayer::HandleClassSelectionOutcome(bool bOutcome)
{
    if (bOutcome)
    {
        if (m_pPlayerPed)
        {
            m_pPlayerPed->ClearWeapons();
            m_pPlayerPed->SetModelIndex(m_SpawnInfo.iSkin);
        }

        m_bClearedToSpawn = true;
    }
}

uint8_t CLocalPlayer::GetSpecialAction()
{
    if(!m_pPlayerPed) return SPECIAL_ACTION_NONE;

    if(m_pPlayerPed->IsCrouching())
        return SPECIAL_ACTION_DUCK;

    if(m_pPlayerPed->IsEnteringVehicle())
        return SPECIAL_ACTION_ENTER_VEHICLE;

    if(m_pPlayerPed->IsExitingVehicle())
        return SPECIAL_ACTION_EXIT_VEHICLE;

    if(m_pPlayerPed->IsSitTask())
        return SPECIAL_ACTION_SITTING;

    if(m_pPlayerPed->IsInJetpackMode())
        return SPECIAL_ACTION_USEJETPACK;

    if(m_pPlayerPed->IsCuffed())
        return SPECIAL_ACTION_CUFFED;

    if(m_pPlayerPed->IsCarry())
        return SPECIAL_ACTION_CARRY;

    if(m_pPlayerPed->GetDanceStyle() != -1)
    {
        switch(m_pPlayerPed->GetDanceStyle())
        {
            case 0:
                return SPECIAL_ACTION_DANCE1;
                break;
            case 1:
                return SPECIAL_ACTION_DANCE2;
                break;
            case 2:
                return SPECIAL_ACTION_DANCE3;
                break;
            case 3:
                return SPECIAL_ACTION_DANCE4;
                break;
        }
    }

    if(m_pPlayerPed->HasHandsUp())
        return SPECIAL_ACTION_HANDSUP;

    if(m_pPlayerPed->IsCellphoneEnabled())
        return SPECIAL_ACTION_USECELLPHONE;

    if(m_pPlayerPed->IsPissing())
        return SPECIAL_ACTION_PISSING;

    if(m_pPlayerPed->GetStuff() == eStuffType::STUFF_TYPE_BEER)
        return SPECIAL_ACTION_DRINK_BEER;

    if(m_pPlayerPed->GetStuff() == eStuffType::STUFF_TYPE_DYN_BEER)
        return SPECIAL_ACTION_DRINK_WINE;

    if(m_pPlayerPed->GetStuff() == eStuffType::STUFF_TYPE_PINT_GLASS)
        return SPECIAL_ACTION_DRINK_SPRUNK;

    if(m_pPlayerPed->GetStuff() == eStuffType::STUFF_TYPE_CIGGI)
        return SPECIAL_ACTION_SMOKE_CIGGY;

    return SPECIAL_ACTION_NONE;
}

uint32_t CLocalPlayer::GetPlayerColor()
{
	return TranslateColorCodeToRGBA(pNetGame->GetPlayerPool()->GetLocalPlayerID());
}

uint32_t CLocalPlayer::GetPlayerColorAsARGB()
{
    return (TranslateColorCodeToRGBA(pNetGame->GetPlayerPool()->GetLocalPlayerID()) >> 8) | 0xFF000000;
}

uint32_t CLocalPlayer::GetPlayerColorAsRGBA()
{
    return TranslateColorCodeToRGBA(pNetGame->GetPlayerPool()->GetLocalPlayerID());
}

bool CLocalPlayer::IsNeedSyncDataSend(const void* data1, const void* data2, size_t size)
{
    if (CTimer::m_snTimeInMillisecondsNonClipped - m_dwLastSendSyncTick <= 500 && !memcmp(data1, data2, size)) {
        return false;
    }

    m_dwLastSendSyncTick = CTimer::m_snTimeInMillisecondsNonClipped;
    return true;
}

void CLocalPlayer::SendEnterVehicleNotification(VEHICLEID VehicleID, bool bPassenger)
{
	RakNet::BitStream bsSend;

	bsSend.Write(VehicleID);
	bsSend.Write((uint8_t)bPassenger);

	pNetGame->GetRakClient()->RPC(&RPC_EnterVehicle, &bsSend, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0,false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::SpectatePlayer(PLAYERID PlayerID)
{
    CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();

    if (pPlayerPool && pPlayerPool->GetSlotState(PlayerID))
    {
        CRemotePlayer* pRemotePlayer = pPlayerPool->GetAt(PlayerID);
        if (pRemotePlayer->GetState() != PLAYER_STATE_NONE &&
            pRemotePlayer->GetState() != PLAYER_STATE_WASTED)
        {
            m_byteSpectateType = SPECTATE_TYPE_PLAYER;
            m_SpectateID = PlayerID;
            m_bSpectateProcessed = false;
        }
    }
}

void CLocalPlayer::SpectateVehicle(VEHICLEID VehicleID)
{
    CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();

    if (pVehiclePool && pVehiclePool->GetSlotState(VehicleID))
    {
        m_byteSpectateType = SPECTATE_TYPE_VEHICLE;
        m_SpectateID = VehicleID;
        m_bSpectateProcessed = false;
    }
}

void CLocalPlayer::SetPlayerColor(uint32_t dwColor)
{
    SetRadarColor(pNetGame->GetPlayerPool()->GetLocalPlayerID(), dwColor);
}

void CLocalPlayer::SendExitVehicleNotification(VEHICLEID VehicleID)
{
    RakNet::BitStream bsSend;

    CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
    CVehicle* pVehicle = pVehiclePool->GetAt(VehicleID);
    if (pVehicle)
    {
        if (!m_pPlayerPed->IsAPassenger())
            m_LastVehicle = VehicleID;

        if (pVehicle->IsATrainPart())
            CCamera::SetBehindPlayer();

        if (!pVehicle->IsRCVehicle())
        {
            bsSend.Write(VehicleID);
            pNetGame->GetRakClient()->RPC(&RPC_ExitVehicle, &bsSend, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
        }
    }
}

void CLocalPlayer::SendTakeDamageEvent(PLAYERID PlayerID, float fDamageFactor, int weaponType, int pedPieceType)
{
    fDamageFactor *= 0.33f;

    RakNet::BitStream bsSend;
    bsSend.Write(true);
    bsSend.Write(PlayerID);
    bsSend.Write(fDamageFactor);
    bsSend.Write(weaponType);
    bsSend.Write(pedPieceType);

    pNetGame->GetRakClient()->RPC(&RPC_PlayerGiveTakeDamage, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::SendGiveDamageEvent(PLAYERID PlayerID, float fDamageFactor, int weaponType, int pedPieceType)
{
    fDamageFactor *= 0.33f;

    RakNet::BitStream bsSend;
    bsSend.Write(false);
    bsSend.Write(PlayerID);
    bsSend.Write(fDamageFactor);
    bsSend.Write(weaponType);
    bsSend.Write(pedPieceType);

    pNetGame->GetRakClient()->RPC(&RPC_PlayerGiveTakeDamage, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::SendGiveDamageActorEvent(PLAYERID ActorID, float fDamageFactor, int weaponType, int pedPieceType)
{
    RakNet::BitStream bsSend;
    bsSend.Write(false);
    bsSend.Write(ActorID);
    bsSend.Write(fDamageFactor);
    bsSend.Write(weaponType);
    bsSend.Write(pedPieceType);

    pNetGame->GetRakClient()->RPC(&RPC_GiveDamageActor, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::GiveTakeDamage(bool bGiveOrTake, uint16_t wPlayerID, float damage_amount, uint32_t weapon_id, uint32_t bodypart)
{
    RakNet::BitStream bitStream;

    bitStream.Write((bool)bGiveOrTake);
    bitStream.Write((uint16_t)wPlayerID);
    bitStream.Write((float)damage_amount);
    bitStream.Write((uint32_t)weapon_id);
    bitStream.Write((uint32_t)bodypart);

    pNetGame->GetRakClient()->RPC(&RPC_PlayerGiveTakeDamage, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::GiveActorDamage(PLAYERID wPlayerID, float damage_amount, uint32_t weapon_id, uint32_t bodypart)
{
    RakNet::BitStream bitStream;
    bitStream.Write((uint16_t)wPlayerID);
    bitStream.Write((float)damage_amount);
    bitStream.Write((uint32_t)weapon_id);
    bitStream.Write((uint32_t)bodypart);

    int RPC_GiveActorDamage = 177;
    pNetGame->GetRakClient()->RPC(&RPC_GiveActorDamage, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

uint32_t CLocalPlayer::GetCurrentAnimationIndexFlag()
{
    uint32_t dwAnim = 0;

    float fBlendData = 4.0f;

    int iAnimIdx = m_pPlayerPed->GetCurrentAnimationIndex();

    uint32_t hardcodedBlend = 0b00000100;	// 4
    hardcodedBlend <<= 16;

    uint32_t hardcodedFlags = 0;

    if (iAnimIdx)
    {
        hardcodedFlags = 0b00010001;
    }
    else
    {
        hardcodedFlags = 0b10000000;
        iAnimIdx = 1189;
    }

    hardcodedFlags <<= 24;

    uint16_t usAnimidx = (uint16_t)iAnimIdx;

    dwAnim = (uint32_t)usAnimidx;
    dwAnim |= hardcodedBlend;
    dwAnim |= hardcodedFlags;

    return dwAnim;
}

void CLocalPlayer::SendBulletSyncData(PLAYERID byteHitID, uint8_t byteHitType, CVector vecHitPos)
{
    if (!m_pPlayerPed) return;
    switch (byteHitType)
    {
        case BULLET_HIT_TYPE_NONE:
            break;
        case BULLET_HIT_TYPE_PLAYER:
            if (!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)byteHitID)) return;
            break;

    }
    uint8_t byteCurrWeapon = m_pPlayerPed->GetCurrentWeapon(), byteShotWeapon;
    //uint8_t byteCurrWeapon = m_pPlayerPed->GetCurrentWeapon(), byteShotWeapon;

    RwMatrix matPlayer;
    BULLET_SYNC blSync;

    matPlayer = m_pPlayerPed->m_pPed->GetMatrix().ToRwMatrix();

    blSync.hitId = byteHitID;
    blSync.hitType = byteHitType;

    if (byteHitType == BULLET_HIT_TYPE_PLAYER)
    {
        float fDistance = pNetGame->GetPlayerPool()->GetAt((PLAYERID)byteHitID)->GetPlayerPed()->m_pPed->GetDistanceFromLocalPlayerPed();
        if (byteCurrWeapon != 0 && fDistance < 1.0f)
            byteShotWeapon = 0;
        else
            byteShotWeapon = byteCurrWeapon;
    }
    else
    {
        byteShotWeapon = m_pPlayerPed->GetCurrentWeapon();
    }
    blSync.weapId = byteShotWeapon;

    blSync.hitPos[0] = vecHitPos.x;
    blSync.hitPos[1] = vecHitPos.y;
    blSync.hitPos[2] = vecHitPos.z;

    blSync.offsets[0] = 0.0f;
    blSync.offsets[1] = 0.0f;
    blSync.offsets[2] = 0.0f;

    FLog("SendBulletSync: %d, %d, %d, %f, %f, %f, %f, %f, %f", blSync.hitId, blSync.hitType, blSync.weapId,
         blSync.hitPos[0], blSync.hitPos[1], blSync.hitPos[2], blSync.offsets[0], blSync.offsets[1], blSync.offsets[2]);

    RakNet::BitStream bsBulletSync;
    bsBulletSync.Write((uint8_t)ID_BULLET_SYNC);
    bsBulletSync.Write((const char*)& blSync, sizeof(BULLET_SYNC));
    pNetGame->GetRakClient()->Send(&bsBulletSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

void CompressNormalVector(CVector *vecOut, CVector vecIn)
{
    vecOut->x = (short)(vecIn.x * 10000.0f);
    vecOut->y = (short)(vecIn.y * 10000.0f);
    vecOut->z = (short)(vecIn.z * 10000.0f);
}

void DecompressNormalVector(RwV3d *vecOut, CVector vecIn)
{
    vecOut->x = (float)(vecIn.x / 10000.0f);
    vecOut->y = (float)(vecIn.y / 10000.0f);
    vecOut->z = (float)(vecIn.z / 10000.0f);
}

int CLocalPlayer::GetOptimumUnoccupiedSendRate()
{
    if(!m_pPlayerPed) return 1000;

    if(m_pPlayerPed->GetGtaVehicle()) // กรณีขับรถชนรถ (Ramming)
        return GetOptimumInCarSendRate();
    else 
        return GetOptimumOnFootSendRate(); // กรณีเดินดันรถ (Pushing)
}

bool CLocalPlayer::ProcessUnoccupiedSync(VEHICLEID vehicleId, CVehicle *pVehicle)
{
    // 1. Rate Limit: ตรวจสอบว่าถึงเวลาส่งของ Global หรือยัง
    if((CTimer::m_snTimeInMillisecondsNonClipped - m_dwLastSendTick) < (unsigned int)GetOptimumUnoccupiedSendRate())
        return false;

    CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
    CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
    if(!pPlayerPool || !pVehiclePool) return false;

    CVehicleGTA *pVehicleType = pVehicle->m_pVehicle;
    
    // 2. Validity Check: รถต้องไม่มีคนขับหลัก (Driver)
    if(pVehicleType && m_pPlayerPed && !pVehicle->IsATrainPart() &&
       !pVehicle->IsATrailer() && !pVehicle->GetTractor())
    {
        // 3. Driver Check: ถ้ามีคนขับอยู่แล้ว (Sync แบบ InCar) ไม่ต้องทำต่อ
        CPedGTA *pDriver = pVehicleType->pDriver;
        if((pDriver && pDriver->IsInVehicle()) ||
           pVehicleType->GetDistanceFromLocalPlayerPed() > 90.0f)
        {
            return false;
        }

        // 4. Passenger Check: ถ้ามีคนนั่งข้างใน ให้คนนั้น Sync (Priority สูงกว่าคนนอกรถ)
        for(int i = 0; i < 7; i++)
        {
            CPedGTA *pPassenger = pVehicleType->m_apPassengers[i];
            if(pPassenger && pPassenger->m_nPedType == (ePedType)0) // 0 = Player Ped
            {
                if(pPassenger == m_pPlayerPed->m_pPed) goto sync; // เราเป็นคนนั่ง เราส่งได้
                return false; // คนอื่นนั่ง ให้เขาส่ง
            }
        }

        // 5. Priority Check (Hybrid Logic): หาคนที่มีสิทธิ์ส่งข้อมูลที่สุด
        float fDistance = 0.0f;
        float fSmallest = 100000.0f;
        PLAYERID iClosestPlayerId = INVALID_PLAYER_ID;
        bool bCollisionFound = false;

        for(PLAYERID i = 0; i < MAX_PLAYERS; i++)
        {
            CPlayerPed* pPlayerPed = NULL;

            if(i == pPlayerPool->GetLocalPlayerID())
                pPlayerPed = m_pPlayerPed;
            else
            {
                if(pPlayerPool->GetSlotState(i))
                {
                    CRemotePlayer* pTmpPlayer = pPlayerPool->GetAt(i);
                    if(pTmpPlayer) pPlayerPed = pTmpPlayer->GetPlayerPed();
                }
            }

            if(pVehicle && pPlayerPed && pPlayerPed->m_pPed->IsAdded())
            {
                fDistance = pPlayerPed->GetDistanceFromVehicle(pVehicle);

                // *** Hybrid Logic Point ***
                // ถ้ามีใครอยู่ใกล้รถมากๆ (< 1.5 เมตร) ถือว่ากำลัง "ดันรถ" หรือ "ชนรถ"
                // ให้สิทธิ์คนนั้นทันที โดยไม่สนว่าคนอื่นจะอยู่ใกล้แค่ไหน (ถ้าเขาไม่ได้ดัน)
                if (!bCollisionFound && fDistance < 1.5f) 
                {
                    fSmallest = fDistance;
                    iClosestPlayerId = i;
                    bCollisionFound = true; // เจอคนดันแล้ว หยุดหาคนอื่น
                }
                else if (!bCollisionFound) // ถ้ายังไม่มีคนดัน ให้หาคนที่ใกล้ที่สุดตามปกติ
                {
                    if(fDistance < fSmallest)
                    {
                        fSmallest = fDistance;
                        iClosestPlayerId = i;
                    }
                }
            }
        }

        // ถ้าเราคือผู้โชคดี (ใกล้สุด หรือ ดันรถอยู่) และระยะไม่เกิน 90 เมตร
        if(iClosestPlayerId == pPlayerPool->GetLocalPlayerID() && fSmallest <= 90.0f)
        {
            sync:
            SendUnoccupiedData(vehicleId, pVehicle);
            m_dwLastSendTick = CTimer::m_snTimeInMillisecondsNonClipped;
            return true;
        }
    }

    return false;
}

void CLocalPlayer::SendUnoccupiedData(VEHICLEID vehicleId, CVehicle *pVehicle)
{
    RakNet::BitStream bsUnoccupiedSync;
    UNOCCUPIED_SYNC_DATA unSync;
    RwMatrix matVehicle;

    // ดึง Matrix จากเกม
    matVehicle = pVehicle->m_pVehicle->GetMatrix().ToRwMatrix();

    // Compress Rotation (อัดข้อมูลทิศทาง)
    CompressNormalVector(&unSync.vecRoll, matVehicle.right);
    CompressNormalVector(&unSync.vecDirection, matVehicle.up);

    unSync.vehicleId = vehicleId;
    unSync.byteSeatId = m_pPlayerPed->GetVehicleSeatID();

    unSync.vecMoveSpeed = pVehicle->m_pVehicle->GetMoveSpeed();
    unSync.vecTurnSpeed = pVehicle->m_pVehicle->GetTurnSpeed();

    unSync.vecPos.x = matVehicle.pos.x;
    unSync.vecPos.y = matVehicle.pos.y;
    unSync.vecPos.z = matVehicle.pos.z;

    unSync.fCarHealth = pVehicle->GetHealth();

    // Bandwidth Optimization: ส่งเฉพาะเมื่อข้อมูลเปลี่ยนไปมากพอ
    if(IsNeedSyncDataSend(&m_UnoccupiedData, &unSync, sizeof(UNOCCUPIED_SYNC_DATA)))
    {
        bsUnoccupiedSync.Write((uint8_t)ID_UNOCCUPIED_SYNC);
        bsUnoccupiedSync.Write((char*)&unSync, sizeof(UNOCCUPIED_SYNC_DATA));
        pNetGame->GetRakClient()->Send(&bsUnoccupiedSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

        // จำค่าล่าสุดไว้เปรียบเทียบครั้งหน้า
        memcpy(&m_UnoccupiedData, &unSync, sizeof(UNOCCUPIED_SYNC_DATA));
    }
}

void CLocalPlayer::MaybeSendExitVehicle()
{
    static bool oldExitVehicleState = false;

    if(!m_pPlayerPed || !m_pPlayerPed->m_pPed) return;

    bool exitVehicleState = m_pPlayerPed->m_pPed->IsExitingVehicle();

    if (exitVehicleState && !oldExitVehicleState)
    {
        CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
        if (pVehiclePool)
        {
            VEHICLEID vehicleId = pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->m_pPed->pVehicle);

            if (vehicleId != INVALID_VEHICLE_ID)
            {
                RakNet::BitStream bsSend;
                bsSend.Write(vehicleId);
                pNetGame->GetRakClient()->RPC(&RPC_ExitVehicle, &bsSend, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
            }
        }
    }
    oldExitVehicleState = exitVehicleState;
}

void CLocalPlayer::MaybeSendEnterVehicle()
{
    static bool oldEnterVehicleState = false;

    if(!m_pPlayerPed || !m_pPlayerPed->m_pPed) return;

    CTaskComplexEnterCarAsDriver* task = static_cast<CTaskComplexEnterCarAsDriver*>(
            m_pPlayerPed->m_pPed->GetTaskManager().CTaskManager::FindActiveTaskByType(TASK_COMPLEX_ENTER_CAR_AS_DRIVER)
    );

    bool enterVehicleState = (task != nullptr);

    if (enterVehicleState && !oldEnterVehicleState)
    {
        CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
        if (pVehiclePool)
        {
            VEHICLEID vehicleId = pVehiclePool->FindIDFromGtaPtr(task->GetTarget());

            if (vehicleId != INVALID_VEHICLE_ID)
                SendEnterVehicleNotification(vehicleId, false);
        }
    }
    oldEnterVehicleState = enterVehicleState;
}