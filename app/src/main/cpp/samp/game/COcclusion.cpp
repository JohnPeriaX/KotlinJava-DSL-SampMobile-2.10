//
// Created by Gor on 1/4/2025.
//

#include "COcclusion.h"
#include "patch.h"

void COcclusion::InjectHooks()
{
    CHook::Write(g_libGTASA+(VER_x32 ? 0xA41140 : 0xCE3EE8), &aOccluders);
    CHook::Write(g_libGTASA+(VER_x32 ? 0xA45790:0xCE8538), &NumOccludersOnMap);
}