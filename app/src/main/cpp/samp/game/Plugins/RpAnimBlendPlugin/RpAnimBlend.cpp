#include "RpAnimBlend.h"
#include "../vendor/armhook/patch.h"

// 0x4D6150
bool RpAnimBlendPluginAttach() {
    return CHook::CallFunction<bool>(g_libGTASA + (VER_x32 ? 0x00390470 + 1 : 0x46A30C));
}

// 0x4D5FA0
void* RtAnimBlendKeyFrameApply(void* result, void* frame) {
    return CHook::CallFunction<void*>(g_libGTASA + (VER_x32 ? 0x003902D8 + 1 : 0x46A1B8), result, frame);

    auto matrix = static_cast<RwMatrix*>(result);
    auto quat = static_cast<RtQuat*>(frame);
}

// 0x4D5F50
CAnimBlendClumpData* RpAnimBlendAllocateData(RpClump* clump) {
    return CHook::CallFunction<CAnimBlendClumpData*>(g_libGTASA + (VER_x32 ? 0x003902B4 + 1 : 0x46A17C), clump);
}

// 0x4D6790
CAnimBlendAssociation* RpAnimBlendClumpAddAssociation(RpClump* clump, CAnimBlendAssociation* association, uint32 flags, float startTime, float blendAmount) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x0039093C + 1 : 0x46A9A0), clump, association, flags, startTime, blendAmount);
}

// 0x4D6BE0
CAnimBlendAssociation* RpAnimBlendClumpExtractAssociations(RpClump* clump) {
    int *v1; // r1
    int v2; // r3
    int result; // r0

    v1 = *(int **)(clump + ClumpOffset);
    v2 = *v1;
    *v1 = 0;
    result = v2 - 4;
    *(uint32_t *)(v2 + 4) = 0;
    return reinterpret_cast<CAnimBlendAssociation *>(result);
}

// 0x4D64A0
void RpAnimBlendClumpFillFrameArray(RpClump* clump, AnimBlendFrameData** frameData) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x00390750 + 1 : 0x46A6F0), clump, frameData);
}

// 0x4D6400
AnimBlendFrameData* RpAnimBlendClumpFindBone(RpClump* clump, uint32 id) {
    return CHook::CallFunction<AnimBlendFrameData*>(g_libGTASA + (VER_x32 ? 0x0039070C + 1 : 0x46A694), clump, id);
}

// 0x4D62A0
AnimBlendFrameData* RpAnimBlendClumpFindFrame(RpClump* clump, const char* name) {
    return CHook::CallFunction<AnimBlendFrameData*>(g_libGTASA + (VER_x32 ? 0x003905C4 + 1 : 0x46A4A4), clump, name);
}

// 0x4D6370
AnimBlendFrameData* RpAnimBlendClumpFindFrameFromHashKey(RpClump* clump, uint32 key) {
    return CHook::CallFunction<AnimBlendFrameData*>(g_libGTASA + (VER_x32 ? 0x0039066C + 1 : 0x46A5A0), clump, key);
}

// 0x4D68E0
CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump* clump, bool bStopFunctionConfusion, CAnimBlendHierarchy* hierarchy) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390A78 + 1 : 0x46AB7C), clump, bStopFunctionConfusion, hierarchy);
}

// 0x4D6870
CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump* clump, const char* name) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390A24 + 1 : 0x46AAF4), clump, name);
}

// AnimationId animId
// 0x4D68B0
CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump* clump, uint32 animId) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390A50 + 1 : 0x46AB48), clump, animId);
}

// 0x4D15E0
CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump* clump) {
    int *v4; // r4
    int result; // r0

    v4 = *(int **)(clump + ClumpOffset);
    result = RpAnimBlendClumpIsInitialized(clump);
    if ( result )
    {
        result = *v4;
        if ( *v4 )
            result -= 4;
    }
    return reinterpret_cast<CAnimBlendAssociation *>(result);
}

// 0x4D6A70
CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump* clump, uint32 flags) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390C04 + 1 : 0x46AD68), clump, flags);
}

// 0x4D6910
CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation(RpClump* clump, CAnimBlendAssociation** pp2ndAnim, float* pBlendVal2nd) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390A9C + 1 : 0x46ABB0), clump, pp2ndAnim, pBlendVal2nd);
}

// 0x4D6A30
CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation_N(RpClump* clump, int32 n) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390B9C + 1 : 0x46ACE8), clump, n);
}

// 0x4D69A0
CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation(RpClump* clump) {
   // assert("RpAnimBlendClumpGetMainPartialAssociation no x64 ver");
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390B44 + 1 : 0x46AC80), clump);
}

// 0x4D69F0
CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation_N(RpClump* clump, int32 n) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390BD0 + 1 : 0x46AD28), clump, n);
}

// 0x4D6B60
uint32 RpAnimBlendClumpGetNumAssociations(RpClump* clump) {
    return CHook::CallFunction<uint32>(g_libGTASA + (VER_x32 ? 0x00390CB0 + 1 : 0x46AE54), clump);
}

// 0x4D6BB0
uint32 RpAnimBlendClumpGetNumNonPartialAssociations(RpClump* clump) {
    return CHook::CallFunction<uint32>(g_libGTASA + (VER_x32 ? 0x00390CF4 + 1 : 0x46AEB4), clump);
}

// 0x4D6B80
uint32 RpAnimBlendClumpGetNumPartialAssociations(RpClump* clump) {
    return CHook::CallFunction<uint32>(g_libGTASA + (VER_x32 ? 0x00390CCC + 1 : 0x46AE78), clump);
}

// 0x4D6C30
void RpAnimBlendClumpGiveAssociations(RpClump* clump, CAnimBlendAssociation* association) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x00390D3C + 1 : 0x46AF18), clump, association);
}

// 0x4D6720
void RpAnimBlendClumpInit(RpClump* clump) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x003907E8 + 1 : 0x46A7D4), clump);
}

// 0x4D6760
bool RpAnimBlendClumpIsInitialized(RpClump* clump) {
//    int result; // r0
//
//    result = *(uint32 *)(clump + ClumpOffset);
//    if ( result )
//        return *(uint32 *)(result + 8) != 0;
//    return result;

   return CHook::CallFunction<bool>(g_libGTASA + (VER_x32 ? 0x39091C + 1 : 0x46A974), clump);
}

// 0x4D6B00
void RpAnimBlendClumpPauseAllAnimations(RpClump* clump) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x00390C60 + 1 : 0x46ADF4), clump);
}

// 0x4D6C00
void RpAnimBlendClumpRemoveAllAssociations(RpClump* clump) {
    RpAnimBlendClumpRemoveAssociations(clump, 0);
}

// 0x4D6820
void RpAnimBlendClumpRemoveAssociations(RpClump* clump, uint32 flags) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x003909EC + 1 : 0x46AA98), clump, flags);
}

// 0x4D67E0
void RpAnimBlendClumpSetBlendDeltas(RpClump* clump, uint32 flags, float delta) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x00390988 + 1 : 0x46AA14), clump, flags, delta);
}

// 0x4D6B30
void RpAnimBlendClumpUnPauseAllAnimations(RpClump* clump) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x00390C88 + 1 : 0x46AE24), clump);
}

// 0x4D34F0
void RpAnimBlendClumpUpdateAnimations(RpClump* clump, float step, bool onScreen) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x0038BF50 + 1 : 0x464624), clump, step, onScreen);
}

// 0x4D60E0
RtAnimAnimation* RpAnimBlendCreateAnimationForHierarchy(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RtAnimAnimation*>(g_libGTASA + (VER_x32 ? 0x00390594 + 1 : 0x46A460), hierarchy);
}

// 0x4D5EF0
char* RpAnimBlendFrameGetName(RwFrame* frame) {
    return CHook::CallFunction<char*>(g_libGTASA + (VER_x32 ? 0x003902A0 + 1 : 0x46A154), frame);
}

// 0x4D5F00
void RpAnimBlendFrameSetName(RwFrame* frame, char* name) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x003902A4 + 1 : 0x46A158), frame, name);
}

// 0x4D6AB0
CAnimBlendAssociation* RpAnimBlendGetNextAssociation(CAnimBlendAssociation* association) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390C34 + 1 : 0x46ADAC), association);
}

// 0x4D6AD0
CAnimBlendAssociation* RpAnimBlendGetNextAssociation(CAnimBlendAssociation* association, uint32 flags) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + (VER_x32 ? 0x00390C3E + 1 : 0x46ADC0), association, flags);
}

// 0x4D60C0
void RpAnimBlendKeyFrameInterpolate(void *pVoidOut, void *pVoidIn1, void *pVoidIn2, RwReal time, void *customData) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x00390440 + 1 : 0x46A2F4), pVoidOut, pVoidIn1, pVoidIn2, time, customData);
}
