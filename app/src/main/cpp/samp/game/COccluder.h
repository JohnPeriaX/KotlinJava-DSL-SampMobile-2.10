//
// Created by Gor on 1/4/2025.
//
#include "../main.h"

class COccluder {
public:
    unsigned short fMidX;
    unsigned short fMidY;
    unsigned short fMidZ;
    unsigned short fWidthX;
    unsigned short fWidthY;
    unsigned short fHeight;
    char cRotZ;
    char cRotY;
    char cRotX;
    char cPad;
    unsigned short nFlags;
};
VALIDATE_SIZE(COccluder, 0x12);
