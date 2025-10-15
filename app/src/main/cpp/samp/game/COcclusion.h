//
// Created by Gor on 1/4/2025.
//

#pragma once
#include <cstdint>
#include "COccluder.h"

class COcclusion {
public:
    static constexpr int32_t MAX_INTERIOR_OCCLUDERS            = 40;
    static constexpr int32_t MAX_MAP_OCCLUDERS                 = 1000;
    static constexpr int32_t MAX_ACTIVE_OCCLUDERS              = 28;
    static constexpr int32_t NUM_OCCLUDERS_PROCESSED_PER_FRAME = 16;

    static inline COccluder aOccluders[MAX_MAP_OCCLUDERS];
    static inline int32 NumOccludersOnMap;

    static void InjectHooks();

};
