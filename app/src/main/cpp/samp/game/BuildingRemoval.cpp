#include "BuildingRemoval.h"
#include "game/Entity/Building.h"
#include "game/Entity/Dummy.h"
#include "game/Entity/Object.h"
#include "game/COcclusion.h"
#include "Pools.h"
#include <cmath>

void CBuildingRemoval::RemoveBuildingByPtr(CEntityGTA* pEntity) {
    if (!pEntity) return;

    CVector newPos = pEntity->GetPosition();
    newPos.z -= 2000.0f;
    pEntity->SetPosn(newPos);

    pEntity->m_bRemoveFromWorld = true;

    if (pEntity->m_matrix) {
        CVector matrixPos = pEntity->m_matrix->GetPosition();
        matrixPos.z -= 2000.0f;
        pEntity->m_matrix->GetPosition() = matrixPos;
    }
}

bool CBuildingRemoval::IsEntityValidForRemoval(CEntityGTA* entity) {
    if (!entity) return false;

    if (entity->m_bRemoveFromWorld || !entity->m_bIsVisible) {
        return false;
    }

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
    RemoveOccluders(pos, 500.0f);

    auto& buildingPool = GetBuildingPool(); // Using accessor from Pools.h
    for (int i = 0; i < buildingPool->GetSize(); i++) {
        CBuilding* building = buildingPool->GetAt(i);
        if (!IsEntityValidForRemoval(building)) continue;

        if (building->m_nModelIndex == modelId || modelId == -1) {
            float distance = GetDistanceBetween3DPoints(&pos, &building->GetPosition());
            if (distance <= radius) {
                RemoveBuildingByPtr(building);
            }
        }
    }

    auto& dummyPool = GetDummyPool();
    for (int i = 0; i < dummyPool->GetSize(); i++) {
        CDummy* dummy = dummyPool->GetAt(i);
        if (!IsEntityValidForRemoval(dummy)) continue;

        if (dummy->m_nModelIndex == modelId || modelId == -1) {
            float distance = GetDistanceBetween3DPoints(&pos, &dummy->GetPosition());
            if (distance <= radius) {
                RemoveBuildingByPtr(dummy);
            }
        }
    }

    auto& objectPool = GetObjectPoolGta();
    for (int i = 0; i < objectPool->GetSize(); i++) {
        CObjectGta* object = objectPool->GetAt(i);
        if (!IsEntityValidForRemoval(object)) continue;

        if (object->m_nModelIndex == modelId || modelId == -1) {
            float distance = GetDistanceBetween3DPoints(&pos, &object->GetPosition());
            if (distance <= radius) {
                RemoveBuildingByPtr(object);
            }
        }
    }
}