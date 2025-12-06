#pragma once

#include "game/Core/Vector.h"
#include "game/Entity/CEntityGTA.h"

struct RemoveBuildingInfo {
    uint32_t modelId;
    CVector position;
    float radius;
};

class CBuildingRemoval {
public:
    static constexpr int MAX_REMOVALS = 1200;
    static inline RemoveBuildingInfo m_RemoveBuildings[MAX_REMOVALS];
    static inline int m_TotalRemovedObjects = 0;

public:
    static void ProcessRemoveBuilding(uint32_t modelId, const CVector& pos, float radius);
    static void RemoveOccluders(const CVector& position, float radius);

    static void RemoveBuildingByPtr(CEntityGTA* pEntity);
    static bool IsEntityValidForRemoval(CEntityGTA* entity);
    static float GetDistanceBetween3DPoints(const CVector* point1, const CVector* point2);
};