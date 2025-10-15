#pragma once

#include "game/Core/Quaternion.h"
#include "../Core/Vector.h"
#include <cstdint>

union Quat16 { // 4096
    struct {
        int16_t m_wQuatX;
        int16_t m_wQuatY;
        int16_t m_wQuatZ;
        int16_t m_wQuatW;
    };
    int16_t rot[4];
};

union Vec16 { // 1024
    struct {
        int16_t m_wTranslationX;
        int16_t m_wTranslationY;
        int16_t m_wTranslationZ;
    };
    int16_t trans[3];
};

/*
 * KeyFrame -> CAnimSequenceChildFrameUncompressed
 * KeyFrameTrans -> CAnimSequenceRootFrameUncompressed
 *
 * KeyFrameCompressed -> CAnimSequenceChildFrameCompressed
 * KeyFrameTransCompressed -> CAnimSequenceRootFrameCompressed
 * */

struct KeyFrame {
    CQuaternion rotation;
    float deltaTime; // relative to previous key frame
};

struct KeyFrameTrans : KeyFrame {
    CVector translation;
};

struct KeyFrameCompressed {
    int16_t rot[4];    // 4096
    int16_t deltaTime; // 60

    void GetRotation(CQuaternion* quat) {
        float scale = 1.0f / 4096.0f;
        quat->x = float(rot[0]) * scale;
        quat->y = float(rot[1]) * scale;
        quat->z = float(rot[2]) * scale;
        quat->w = float(rot[3]) * scale;
    }

    void SetRotation(const CQuaternion& quat) {
        rot[0] = int16_t(quat.x * 4096.0f);
        rot[1] = int16_t(quat.y * 4096.0f);
        rot[2] = int16_t(quat.z * 4096.0f);
        rot[3] = int16_t(quat.w * 4096.0f);
    }

    [[nodiscard]] float GetDeltaTime() const {
        return (float)deltaTime / 60.0f;
    }

    void SetTime(float time) {
        deltaTime = int16_t(time * 60.0f + 0.5f);
    }
};

struct KeyFrameTransCompressed : KeyFrameCompressed {
    int16_t trans[3]; // 1024

    void GetTranslation(CVector* vec) {
        float scale = 1.0f / 1024.0f;
        vec->x = float(trans[0]) * scale;
        vec->y = float(trans[1]) * scale;
        vec->z = float(trans[2]) * scale;
    }

    CVector GetTranslation() {
        CVector out;
        GetTranslation(&out);
        return out;
    }

    void SetTranslation(const CVector& vec) {
        trans[0] = int16_t(vec.x * 1024.0f);
        trans[1] = int16_t(vec.y * 1024.0f);
        trans[2] = int16_t(vec.z * 1024.0f);
    }
};

static_assert(sizeof(KeyFrameCompressed)==0xA);
static_assert(sizeof(KeyFrameTransCompressed)==0x10);
static_assert(sizeof(KeyFrame)==0x14);
static_assert(sizeof(KeyFrameTrans)==0x20);
