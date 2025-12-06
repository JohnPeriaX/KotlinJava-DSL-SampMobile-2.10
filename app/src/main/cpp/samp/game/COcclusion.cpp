//
// Created by Gor on 1/4/2025.
//

#include "COcclusion.h"
#include "patch.h"

void COcclusion::InjectHooks()
{
    CHook::Write(g_libGTASA+(VER_x32 ? 0x67843C : 0x84E8A0), &aOccluders); // 0xA41140 0xCE3EE8 old
    CHook::Write(g_libGTASA+(VER_x32 ? 0x678164 : 0x84E2F0), &NumOccludersOnMap); // 0xA45790 0xCE8538 old
}