/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "../common.h"
#include "../RW/rpworld.h"
#include "AnimBlendHierarchy.h"

class CAnimBlendSequence;

class CAnimBlendStaticAssociation {
public:
    uintptr              vtable;
    uint16               m_nNumBlendNodes;
    int16                m_nAnimId;
    int16                m_nAnimGroup;
    uint16               m_nFlags;
    CAnimBlendSequence** m_pSequenceArray;
    CAnimBlendHierarchy* m_pHierarchy;

public:
    static void InjectHooks();

    CAnimBlendStaticAssociation();
    CAnimBlendStaticAssociation(RpClump* clump, CAnimBlendHierarchy* hier);
  //  virtual ~CAnimBlendStaticAssociation();

    void Init(RpClump* clump, CAnimBlendHierarchy* hier);
    void AllocateSequenceArray(int32 count);
    void FreeSequenceArray();

    auto GetHashKey() const noexcept { return m_pHierarchy->m_hashKey; }
};

VALIDATE_SIZE(CAnimBlendStaticAssociation, (VER_x32 ? 0x14 : 0x20));
