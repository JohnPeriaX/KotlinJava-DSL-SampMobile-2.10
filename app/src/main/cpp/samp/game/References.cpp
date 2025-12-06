#include "common.h"
#include "References.h"
#include "../vendor/armhook/patch.h"
#include "Pools.h"
#include "World.h"

void CReferences::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x676DC0 : 0x84BBE0), &CReferences::pEmptyList);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x67901C : 0x850068), &CReferences::aRefs);
}

// 0x5719B0
void CReferences::Init() {
    pEmptyList = aRefs;

    // todo: do better
    for (int32 i = 0; i < MAX_NUM_REFERENCES - 1; ++i) {
        aRefs[i].m_pNext = &aRefs[i + 1];
        aRefs[i].m_ppEntity = nullptr;
    }

    aRefs[MAX_NUM_REFERENCES - 1].m_pNext = nullptr;
}

uint32 CReferences::ListSize(CReference* ref) {
    uint32 count = 0;
    while (ref) {
        ref = ref->m_pNext;
        ++count;
    }

    return count;
}

void CReferences::RemoveReferencesToPlayer() {
    auto vehicle = FindPlayerVehicle();
    if (vehicle)
        vehicle->ResolveReferences();

    auto player = FindPlayerPed();
    if (player) {
        player->ResolveReferences();
//        auto& group = CPedGroups::GetGroup(player->m_pPlayerData->m_nPlayerGroup);
//        group.GetMembership().SetLeader(player);
//        group.Process();
    }
}

void CReferences::PruneAllReferencesInWorld() {
    for (int32 i = 0; i < GetPedPoolGta()->GetSize(); ++i) {
        if (auto ped = GetPedPoolGta()->GetAt(i))
            ped->PruneReferences();
    }

    for (int32 i = 0; i < GetVehiclePoolGta()->GetSize(); ++i) {
        if (auto vehicle = GetVehiclePoolGta()->GetAt(i))
            vehicle->PruneReferences();
    }

    for (int32 i = 0; i < GetObjectPoolGta()->GetSize(); ++i) {
        if (auto obj = GetObjectPoolGta()->GetAt(i))
            obj->PruneReferences();
    }
}