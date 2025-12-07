#include "BuildingRemoval.h"
#include "game/Entity/Building.h"
#include "game/Entity/Dummy.h"
#include "game/Entity/Object.h"
#include "game/COcclusion.h"
#include "Pools.h"
#include "util/CUtil.h"
#include <cmath>

void CBuildingRemoval::RemoveBuildingByPtr(CEntityGTA* pEntity) {
    if (!pEntity) return;

    // Move entity underground
    CVector newPos = pEntity->GetPosition();
    newPos.z -= 2000.0f;
    pEntity->SetPosn(newPos);

    // Set removal flags
    pEntity->m_bRemoveFromWorld = true;
    pEntity->m_nAreaCode = AREA_CODE_1;
    pEntity->m_bUsesCollision = false;

    // Handle matrix position
    if (pEntity->m_matrix) {
        CVector& matrixPos = pEntity->m_matrix->GetPosition();
        matrixPos.z -= 2000.0f;
    }
}

bool CBuildingRemoval::IsEntityValidForRemoval(CEntityGTA* entity) {
    if (!entity) return false;

    // Check if already removed or not visible
    if (entity->m_bRemoveFromWorld || !entity->m_bIsVisible) {
        return false;
    }

    // VTable validation for additional safety (optional but recommended)
    auto vtable = *reinterpret_cast<uintptr_t*>(entity);
    vtable -= g_libGTASA;

    // Skip if VTable matches CPlaceable (0x667D14 for x32, 0x830098 for x64)
    #if VER_x32
    if (vtable == 0x00667D14) return false;
    #else
    if (vtable == 0x830098) return false;
    #endif

    return true;
}

float CBuildingRemoval::GetDistanceBetween3DPoints(const CVector* point1, const CVector* point2) {
    if (!point1 || !point2) return NAN; // Return NAN or max float

    float dx = point1->x - point2->x;
    float dy = point1->y - point2->y;
    float dz = point1->z - point2->z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

void CBuildingRemoval::RemoveOccluders(const CVector& position, float radius) {
    for (int32_t i = 0; i < COcclusion::NumOccludersOnMap; i++) {
        COccluder& occluder = COcclusion::aOccluders[i];

        CVector occluderPos;
        occluderPos.x = (float)occluder.iCenterX * 0.25f;
        occluderPos.y = (float)occluder.iCenterY * 0.25f;
        occluderPos.z = (float)occluder.iCenterZ * 0.25f;

        if (GetDistanceBetween3DPoints(&position, &occluderPos) < radius) {
            occluder.iCenterX = 0;
            occluder.iCenterY = 0;
            occluder.iCenterZ = 0;

            occluder.iLength = 0;
            occluder.iWidth = 0;
            occluder.iHeight = 0;

            occluder.iRotX = 0;
            occluder.iRotY = 0;
            occluder.iRotZ = 0;
        }
    }
}

void CBuildingRemoval::ProcessRemoveBuilding(uint32_t modelId, const CVector& pos, float radius) {
    // Remove occluders with larger radius for safety
    RemoveOccluders(pos, 500.0f);

    // Use template function for all pools
    RemoveBuildingsInPool(GetBuildingPool(), modelId, pos, radius);
    RemoveBuildingsInPool(GetDummyPool(), modelId, pos, radius);
    RemoveBuildingsInPool(GetObjectPoolGta(), modelId, pos, radius);
}

// Template implementation for pool processing
template <typename PoolT>
void CBuildingRemoval::RemoveBuildingsInPool(PoolT* pool, uint32_t uiModel, const CVector& pos, float radius) {
    if (!pool) return;

    for (int i = 0; i < pool->GetSize(); i++) {
        auto* entity = pool->GetAt(i);
        if (!IsEntityValidForRemoval(entity)) continue;

        // Check model match (or -1 for all models)
        if (entity->m_nModelIndex == uiModel || uiModel == static_cast<uint32_t>(-1)) {
            float distance = GetDistanceBetween3DPoints(&pos, &entity->GetPosition());
            if (distance <= radius) {
                RemoveBuildingByPtr(entity);
            }
        }
    }
}

// Explicit template instantiations
template void CBuildingRemoval::RemoveBuildingsInPool<CPool<CBuilding>>(CPool<CBuilding>*, uint32_t, const CVector&, float);
template void CBuildingRemoval::RemoveBuildingsInPool<CPool<CDummy>>(CPool<CDummy>*, uint32_t, const CVector&, float);
template void CBuildingRemoval::RemoveBuildingsInPool<CPool<CObjectGta>>(CPool<CObjectGta>*, uint32_t, const CVector&, float);