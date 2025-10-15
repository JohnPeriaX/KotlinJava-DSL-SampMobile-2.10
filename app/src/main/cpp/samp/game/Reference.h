/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

//struct CEntity;

class CReference {
public:
    class CReference*       m_pNext;
    class CEntityGTA**      m_ppEntity;
};

static_assert(sizeof(CReference) == (VER_x32 ? 0x8 : 0x10), "Invalid size CReference");
