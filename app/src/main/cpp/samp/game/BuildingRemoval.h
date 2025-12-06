//
// Created by unk.cs on 31/01/2025.
//

#pragma once

#include "Common.h"

constexpr int MAX_REMOVE_BUILDINGS = 1200;

extern int RemoveModelIDs[MAX_REMOVE_BUILDINGS];
extern CVector RemovePos[MAX_REMOVE_BUILDINGS];
extern float RemoveRad[MAX_REMOVE_BUILDINGS];
extern int iTotalRemovedObjects;

class CBuildingRemoval {
public:
    static void ProcessRemoveBuilding(int uModelID, CVector pos, float fRad);
};