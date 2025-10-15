#pragma once


#include "common.h"
#include "../vendor/armhook/patch.h"

enum class eCdStreamStatus : int32 {
    READING_SUCCESS = 0,
    WAITING_TO_READ = 250,
    READING_FAILURE = 254,
    READING = 255,
};

void CdStreamInit(int32 streamCount);
int32 CdStreamOpen(const char* lpFileName);
eCdStreamStatus CdStreamSync(int32 streamId);
eCdStreamStatus CdStreamGetStatus(int32 streamId);
bool CdStreamRead(int32 streamId, void* lpBuffer, uint32 offsetAndHandle, int32 sectorCount);
static uint32 CdStreamGetLastPosn() { return CHook::CallFunction<uint32>(g_libGTASA + (VER_x32 ? 0x2C9C30 + 1 : 0x38AE04)); }