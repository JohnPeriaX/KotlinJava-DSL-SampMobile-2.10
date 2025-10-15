/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "Core/Rect.h"

struct IplDef {
    CRect bb{};
    char  name[18]{};

    int16_t firstBuilding{ SHRT_MAX };
    int16_t lastBuilding{ SHRT_MIN };

    int16_t firstDummy{ SHRT_MAX };
    int16_t lastDummy{ SHRT_MIN };

    int16_t staticIdx{ -1 }; // entity arrays index
    bool    isInterior{};
    char    loaded{};
    bool    required{};
    bool    ignore{ true };
    char    ignoreWhenDeleted{};
    char    isLarge{}; // Makes bounding box bigger. (+350 vs +200 units). See `CIplStore::LoadIpl`

    uint8_t pad_0[2];

    constexpr IplDef() = default;

    IplDef(const char* name) {
        strcpy(this->name, name);
    }
};
static_assert(sizeof(IplDef) == 0x34);
