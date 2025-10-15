#pragma once

struct AnimDescriptor {
    int32 animId{};
    int32 flags{};
};

struct AnimAssocDefinition {
    constexpr static size_t ANIM_NAME_BUF_SZ = 24;

    char   groupName[16]{};
    char   blockName[16]{};
    int32  modelIndex{};
    int32  animsCount{};

    const char**    animNames{};
    AnimDescriptor* animDesc{};
};
VALIDATE_SIZE(AnimAssocDefinition, (VER_x32 ? 0x30 : 0x38));
