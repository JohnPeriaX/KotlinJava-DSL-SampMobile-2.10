//
// Created by Gor on 1/4/2025.
//
#include "../main.h"

struct COccluder
{
    int16 iCenterX;  // 0x00
    int16 iCenterY;  // 0x02
    int16 iCenterZ;  // 0x04
    int16 iLength;   // 0x06
    int16 iWidth;    // 0x08
    int16 iHeight;   // 0x0A
    uint8 iRotZ;     // 0x0C
    uint8 iRotY;     // 0x0D
    uint8 iRotX;     // 0x0E
    uint8 padding_COccluder;   // 0x0F (padding byte)
    struct {
        int16 NextInList : 15;
        int16 bDontStream : 1;
    };
};
VALIDATE_SIZE(COccluder, 0x12);