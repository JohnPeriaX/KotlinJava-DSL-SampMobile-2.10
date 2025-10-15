//
// Created by roman on 11/24/2024.
//

#pragma once


#include "CEntityGTA.h"
#include "PtrNodeDoubleLink.h"
#include "EntryInfoList.h"
#include "game/RealTimeShadow.h"

struct CPhysical : public CEntityGTA {
    float       m_fPrevDistFromCam;
    uint32_t    m_LastCollisionTime;
    union {
        struct {
            uint32_t bMakeMassTwiceAsBig : 1;
            uint32_t bApplyGravity : 1;
            uint32_t bDisableCollisionForce : 1;
            uint32_t bCollidable : 1;
            uint32_t bDisableTurnForce : 1;
            uint32_t bDisableMoveForce : 1;
            uint32_t bInfiniteMass : 1;
            uint32_t bDisableZ : 1;

            uint32_t bSubmergedInWater : 1;
            uint32_t bOnSolidSurface : 1;
            uint32_t bBroken : 1;
            uint32_t bProcessCollisionEvenIfStationary : 1; // ref @ 0x6F5CF0
            uint32_t bSkipLineCol : 1;                               // only used for peds
            uint32_t bDontApplySpeed : 1;
            uint32_t b15 : 1;
            uint32_t bProcessingShift : 1;

            uint32_t b17 : 1;
            uint32_t bDisableSimpleCollision : 1; // ref @ CPhysical::ProcessCollision
            uint32_t bBulletProof : 1;
            uint32_t bFireProof : 1;
            uint32_t bCollisionProof : 1;
            uint32_t bMeleeProof : 1;
            uint32_t bInvulnerable : 1;
            uint32_t bExplosionProof : 1;

            uint32_t bDontCollideWithFlyers : 1;
            uint32_t bAttachedToEntity : 1;
            uint32_t bAddMovingCollisionSpeed : 1;
            uint32_t bTouchingWater : 1;
            uint32_t bCanBeCollidedWith : 1;
            uint32_t bDestroyed : 1;
            uint32_t b31 : 1;
            uint32_t b32 : 1;
        } physicalFlags;
        uint32_t m_nPhysicalFlags;
    };
    CVector         m_vecMoveSpeed;
    CVector         m_vecTurnSpeed;
    CVector         m_vecFrictionMoveSpeed;
    CVector         m_vecFrictionTurnSpeed;
    CVector         m_vecForce;
    CVector         m_vecTorque;
    float           m_fMass;
    float           m_fTurnMass;
    float           m_fVelocityFrequency;
    float           m_fAirResistance;
    float           m_fElasticity;
    float           m_fBuoyancyConstant;
    CVector         m_vecCentreOfMass;
    CEntryInfoList       m_pCollisionList;

    uintptr_t       m_pMovingList;

    uint8_t         m_nFakePhysics;
    uint8_t         m_nNumEntitiesCollided;
    eSurfaceType    m_nContactSurface;
    uint8_t         _pad4;

    CEntityGTA*        m_apCollidedEntities[6];
    float           m_fMovingSpeed; // m_fTrueDistanceTravelled
    float           m_fDamageIntensity; // m_fDamageImpulseMagnitude
    CEntityGTA         *m_pDamageEntity;
    CVector         m_vecLastCollisionImpactVelocity;
    CVector         m_vecLastCollisionPosn;
    uint16_t        m_nPieceType;
    uint8_t         _pad5[2];
    CEntityGTA         *m_pAttachedTo;
    CVector         m_vecAttachPosnOffset;
    CVector         m_vecAttachTiltOffset;
    CQuaternion     m_qAttachedEntityRotation;
    CEntityGTA         *m_pEntityIgnoredCollision;
    float           m_fContactSurfaceBrightness;
    float           m_fDynamicLighting;
    CRealTimeShadow*       m_pShadowData;

public:
    CPhysical();
    ~CPhysical() override;

    // originally virtual functions
    void Add() override;
    void Remove() override;
    bool IsAdded();

    CVector GetSpeed( CVector point);
    void ApplyMoveSpeed();
    void ApplyTurnSpeed();

    CVector& GetMoveSpeed()                 { return m_vecMoveSpeed; }
    void     SetVelocity(CVector velocity)  { m_vecMoveSpeed = velocity; } // 0x441130

    CVector& GetTurnSpeed()         { return m_vecTurnSpeed; }
    void ResetTurnSpeed()           { m_vecTurnSpeed = CVector(); }
    void SetTurnSpeed(CVector vec)  { m_vecTurnSpeed = vec; }

    void ResetMoveSpeed()           { SetVelocity(CVector{}); }

    void ApplyMoveForce(float x, float y, float z);
    void ApplyMoveForce(CVector force);
    void ApplyTurnForce(CVector force, CVector point);
};

static_assert(sizeof(CPhysical) == (VER_x32 ? 0x13C : 0x198), "Invalid size CPhysical");