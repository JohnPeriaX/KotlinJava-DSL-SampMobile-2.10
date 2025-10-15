
#include "StreamingInfo.h"
#include "Streaming.h"
#include "../vendor/armhook/patch.h"

void CStreamingInfo::InjectHooks() {

    CHook::Write(g_libGTASA + (VER_x32 ? 0x00678578 : 0x84EB18), &CStreamingInfo::ms_pArrayBase);
}

// 0x407460
void CStreamingInfo::Init() {
    m_nLoadState = LOADSTATE_NOT_LOADED;
    m_nNextIndex = -1;
    m_nPrevIndex = -1;
    m_nNextIndexOnCd = -1;
    m_nImgId = 0;
    m_nCdSize = 0;
    m_nCdPosn = 0;
}

// 0x407570
size_t CStreamingInfo::GetCdPosn() const {
    return m_nCdPosn + CStreaming::ms_files[m_nImgId].m_StreamHandle;
}

// 0x4075E0
void CStreamingInfo::SetCdPosnAndSize(size_t CdPosn, size_t CdSize) {
    m_nCdPosn = CdPosn;
    m_nCdSize = CdSize;
}

// 0x4075A0
bool CStreamingInfo::GetCdPosnAndSize(size_t& CdPosn, size_t& CdSize) {
    if (!HasCdPosnAndSize())
        return false;
    CdPosn = GetCdPosn();
    CdSize = m_nCdSize;
    return true;
}

// 0x407560
bool CStreamingInfo::InList() const {
    // Yeah, that's partially true
    // Because the way these lists work, items actually always have both `next` and `prev` defined
    // So, I guess here they just assume that, and "optimize" the check :D
    return m_nNextIndex != -1 /*notsa => */ && m_nPrevIndex != -1;
}

// 0x407480
void CStreamingInfo::AddToList(CStreamingInfo* after) {
    assert(!InList()); // May not be in a list (As that would corrupt the list)

    m_nNextIndex = after->m_nNextIndex;
    m_nPrevIndex = static_cast<int16>(after - ms_pArrayBase);

    after->m_nNextIndex = static_cast<int16>(this - ms_pArrayBase);
    ms_pArrayBase[m_nNextIndex].m_nPrevIndex = after->m_nNextIndex;
}

// 0x4074E0
void CStreamingInfo::RemoveFromList() {
    assert(InList()); // Must be in a list (Otherwise array access is UB)

    ms_pArrayBase[m_nNextIndex].m_nPrevIndex = m_nPrevIndex;
    ms_pArrayBase[m_nPrevIndex].m_nNextIndex = m_nNextIndex;
    m_nNextIndex = -1;
    m_nPrevIndex = -1;
}
