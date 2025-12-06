//
// Created by unk.cs on 31/01/2025.
//

#include "BuildingRemoval.h"
#include "game/Entity/CEntityGTA.h"
#include "util/CUtil.h"
#include "Pools.h"

int RemoveModelIDs[MAX_REMOVE_BUILDINGS]{};
CVector RemovePos[MAX_REMOVE_BUILDINGS]{};
float RemoveRad[MAX_REMOVE_BUILDINGS]{};
int iTotalRemovedObjects = 0;

void RemoveBuildingByEntity(CEntityGTA* entity)
{
    CVector pos = entity->m_placement.m_vPosn;
    pos.z -= 2000.0f;

    entity->m_nAreaCode = AREA_CODE_1;

    entity->m_bUsesCollision = false;

    CMatrixLink* pMatrix = entity->m_matrix;

    if (pMatrix && pMatrix->GetRight() != 0xFFFFFF && pMatrix->GetRight() != static_cast<float>(0xFFFFFFFF)) {
        pMatrix->GetRight().z -= 2000.0f;
    }
}

template <typename PoolT>
void RemoveBuildingsInPool(PoolT* pool, int uiModel, CVector pos, float radius)
{
    if(!pool)
        return;

    for (auto i = 0; i < pool->GetSize(); i++)
    {
        auto* entity = pool->GetAt(i);

        if (!entity)
            continue;

        auto vtable = *reinterpret_cast<uintptr*>(entity);
        vtable -= g_libGTASA;

        if (vtable != (VER_x32 ? 0x00667D14 : 0x830098) && (!pool->m_byteMap[i].bEmpty) && (entity->m_nModelIndex == uiModel || uiModel == -1))
        {
            CVector EntityPos = entity->m_placement.m_vPosn;

            if (CUtil::GetDistanceBetween3DPoints(pos, EntityPos) <= radius) {
                RemoveBuildingByEntity(entity);
            }
        }
    }
}

void RemoveOccluders(float targetX, float targetY, float targetZ, float targetRadius)
{
    auto* numOccluders = (uintptr_t*)(g_libGTASA + (VER_x32 ? 0xA45790 : 0xCE8538));

    if (*numOccluders > 0)
    {
        char* occluderData = (char*)(g_libGTASA + (VER_x32 ? 0xA41140 + 0x4 : 0xCE3EE8 + 0x4));

        for (int i = 0; i < *numOccluders; i++)
        {
            double occluderX = (double)*(int16_t*)occluderData * 0.25;
            double occluderY = (double)*((int16_t*)occluderData - 1) * 0.25;
            double occluderZ = (double)*((int16_t*)occluderData - 2) * 0.25;

            CVector occluderPos = { (float)occluderZ, (float)occluderY, (float)occluderX };
            CVector targetPos = { targetX, targetY, targetZ };

            if (CUtil::GetDistanceBetween3DPoints(occluderPos, targetPos) < targetRadius)
            {
                *((int16_t*)occluderData - 2) = 0;
                *((int16_t*)occluderData - 1) = 0;
                *(int16_t*)occluderData = 0;
                *((int16_t*)occluderData + 1) = 0;
                *((int16_t*)occluderData + 2) = 0;
                *((int16_t*)occluderData + 3) = 0;
            }

            occluderData += 0x12;
        }
    }
}

void CBuildingRemoval::ProcessRemoveBuilding(int uModelID, CVector pos, float radius)
{
    RemoveOccluders(pos.x, pos.y, pos.z, 500.0);

    RemoveBuildingsInPool(GetBuildingPool(), uModelID, pos, radius);
    RemoveBuildingsInPool(GetDummyPool(), uModelID, pos, radius);
    RemoveBuildingsInPool(GetObjectPoolGta(), uModelID, pos, radius);
}