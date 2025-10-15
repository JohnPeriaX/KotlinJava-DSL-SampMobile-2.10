#include "main.h"
#include "patch.h"

#define FLAG_ALPHA_TEST           0x01
#define FLAG_LIGHTING             0x02
#define FLAG_ALPHA_MODULATE       0x04
#define FLAG_COLOR_EMISSIVE       0x08
#define FLAG_COLOR                0x10
#define FLAG_TEX0                 0x20
#define FLAG_ENVMAP               0x40          // normal envmap
#define FLAG_BONE3                0x80
#define FLAG_BONE4                0x100
#define FLAG_CAMERA_BASED_NORMALS 0x200
#define FLAG_FOG                  0x400
#define FLAG_TEXBIAS              0x800
#define FLAG_BACKLIGHT            0x1000
#define FLAG_LIGHT1               0x2000
#define FLAG_LIGHT2               0x4000
#define FLAG_LIGHT3               0x8000
#define FLAG_DETAILMAP            0x10000
#define FLAG_COMPRESSED_TEXCOORD  0x20000
#define FLAG_PROJECT_TEXCOORD     0x40000
#define FLAG_WATER                0x80000
#define FLAG_COLOR2               0x100000
#define FLAG_SPHERE_XFORM         0x800000      // this renders the scene as a sphere map for vehicle reflections
#define FLAG_SPHERE_ENVMAP        0x1000000     // spherical real-time envmap
#define FLAG_TEXMATRIX            0x2000000
#define FLAG_GAMMA                0x4000000

#define PXL_EMIT(...)                        \
  do {                                       \
    snprintf(tmp, sizeof(tmp), __VA_ARGS__); \
    strcat(pxlbuf, tmp);                     \
    strcat(pxlbuf, "\n");                    \
  } while (0)


#define VTX_EMIT(...)                        \
  do {                                       \
    snprintf(tmp, sizeof(tmp), __VA_ARGS__); \
    strcat(vtxbuf, tmp);                     \
    strcat(vtxbuf, "\n");                    \
  } while (0)

char pxlbuf[1024 * 10];
char vtxbuf[1024 * 10];

char* byte_617290 = 0; //pxl
char* byte_617258 = 0;

char* byte_619294 = 0; //vertex

#define PXL_SOURCE(str) \
		strcat(byte_617290, str) \

#define VTX_SOURCE(str) \
        strcat(byte_619294, str) \


void BuildVertexSource(int flags) {
    const char *v4; // x8
    char v9[512]; // [xsp+8h] [xbp-458h] BYREF
    char s[8]; // [xsp+208h] [xbp-258h] BYREF

    snprintf(s, 0x200uLL, "#version 100\n");
    strcat((char *)&vtxbuf, s);
    snprintf(s, 0x200uLL, "precision highp float;");
    strcat((char *)&vtxbuf, s);
    snprintf(s, 0x200uLL, "uniform mat4 ProjMatrix;");
    strcat((char *)&vtxbuf, s);
    snprintf(s, 0x200uLL, "uniform mat4 ViewMatrix;");
    strcat((char *)&vtxbuf, s);
    snprintf(s, 0x200uLL, "uniform mat4 ObjMatrix;");
    strcat((char *)&vtxbuf, s);
    if ( (flags & 2) != 0 )
    {
        snprintf(s, 0x200uLL, "uniform lowp vec3 AmbientLightColor;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "uniform lowp vec4 MaterialEmissive;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "uniform lowp vec4 MaterialAmbient;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "uniform lowp vec4 MaterialDiffuse;");
        strcat((char *)&vtxbuf, s);
        if ( (flags & 0x2000) != 0 )
        {
            snprintf(s, 0x200uLL, "uniform lowp vec3 DirLightDiffuseColor;");
            strcat((char *)&vtxbuf, s);
            snprintf(s, 0x200uLL, "uniform vec3 DirLightDirection;");
            strcat((char *)&vtxbuf, s);
            if ( (flags & 0x1180) != 0 && CHook::CallFunction<int>("_Z22GetMobileEffectSettingv") == 3 )
            {
                snprintf(s, 0x200uLL, "uniform vec3 DirBackLightDirection;");
                strcat((char *)&vtxbuf, s);
            }
        }
        if ( (flags & 0x4000) != 0 )
        {
            snprintf(s, 0x200uLL, "uniform lowp vec3 DirLight2DiffuseColor;");
            strcat((char *)&vtxbuf, s);
            snprintf(s, 0x200uLL, "uniform vec3 DirLight2Direction;");
            strcat((char *)&vtxbuf, s);
        }
        if ( (flags & 0x8000) != 0 )
        {
            snprintf(s, 0x200uLL, "uniform lowp vec3 DirLight3DiffuseColor;");
            strcat((char *)&vtxbuf, s);
            snprintf(s, 0x200uLL, "uniform vec3 DirLight3Direction;");
            strcat((char *)&vtxbuf, s);
        }
    }
    snprintf(s, 0x200uLL, "attribute vec3 Position;");
    strcat((char *)&vtxbuf, s);
    snprintf(s, 0x200uLL, "attribute vec3 Normal;");
    strcat((char *)&vtxbuf, s);
    if ( (flags & 0x20) != 0 )
    {
        if ( (flags & 0x40000) != 0 )
            snprintf(s, 0x200uLL, "attribute vec4 TexCoord0;");
        else
            snprintf(s, 0x200uLL, "attribute vec2 TexCoord0;");
        strcat((char *)&vtxbuf, s);
    }
    snprintf(s, 0x200uLL, "attribute vec4 GlobalColor;");
    strcat((char *)&vtxbuf, s);
    if ( (flags & 0x180) != 0 )
    {
        snprintf(s, 0x200uLL, "attribute vec4 BoneWeight;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "attribute vec4 BoneIndices;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "uniform highp vec4 Bones[%d];", 3 * *(uint32_t *)(g_libGTASA + (VER_x32 ? 0x6B8BAC:0x896140)));
        strcat((char *)&vtxbuf, s);
    }
    if ( (flags & 0x20) != 0 )
    {
        snprintf(s, 0x200uLL, "varying mediump vec2 Out_Tex0;");
        strcat((char *)&vtxbuf, s);
    }
    if ( (flags & 0x2000000) != 0 )
    {
        snprintf(s, 0x200uLL, "uniform mat3 NormalMatrix;");
        strcat((char *)&vtxbuf, s);
    }
    if ( (flags & 0x1000040) != 0 )
    {
        if ( (flags & 0x40) != 0 )
            snprintf(s, 0x200uLL, "varying mediump vec2 Out_Tex1;");
        else
            snprintf(s, 0x200uLL, "varying mediump vec3 Out_Refl;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "uniform lowp float EnvMapCoefficient;");
        strcat((char *)&vtxbuf, s);
    }
    if ( (flags & 0x18807C0) != 0 )
    {
        snprintf(s, 0x200uLL, "uniform vec3 CameraPosition;");
        strcat((char *)&vtxbuf, s);
    }
    if ( (flags & 0x400) != 0 )
    {
        snprintf(s, 0x200uLL, "varying mediump float Out_FogAmt;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "uniform vec3 FogDistances;");
        strcat((char *)&vtxbuf, s);
        if ( (flags & 0x80000) == 0 )
        {
            LABEL_30:
            if ( (flags & 0x100000) == 0 )
                goto LABEL_32;
            goto LABEL_31;
        }
    }
    else if ( (flags & 0x80000) == 0 )
    {
        goto LABEL_30;
    }
    snprintf(s, 0x200uLL, "uniform vec3 WaterSpecs;");
    strcat((char *)&vtxbuf, s);
    snprintf(s, 0x200uLL, "varying mediump vec2 Out_WaterDetail;");
    strcat((char *)&vtxbuf, s);
    snprintf(s, 0x200uLL, "varying mediump vec2 Out_WaterDetail2;");
    strcat((char *)&vtxbuf, s);
    snprintf(s, 0x200uLL, "varying mediump float Out_WaterAlphaBlend;");
    strcat((char *)&vtxbuf, s);
    if ( (flags & 0x100000) != 0 )
    {
        LABEL_31:
        snprintf(s, 0x200uLL, "attribute vec4 Color2;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "uniform lowp float ColorInterp;");
        strcat((char *)&vtxbuf, s);
    }
    LABEL_32:
    if ( (flags & 0x12) != 0 )
    {
        snprintf(s, 0x200uLL, "varying lowp vec4 Out_Color;");
        strcat((char *)&vtxbuf, s);
    }
    if ( (flags & 0x10001C0) != 0 && (flags & 0x2000) != 0 )
    {
        snprintf(s, 0x200uLL, "varying lowp vec3 Out_Spec;");
        strcat((char *)&vtxbuf, s);
    }
    snprintf(s, 0x200uLL, "void main() {");
    strcat((char *)&vtxbuf, s);
    if ( (flags & 0x180) != 0 )
    {
        snprintf(s, 0x200uLL, "\tivec4 BlendIndexArray = ivec4(BoneIndices);");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tmat4 BoneToLocal;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[0] = Bones[BlendIndexArray.x*3] * BoneWeight.x;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[1] = Bones[BlendIndexArray.x*3+1] * BoneWeight.x;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[2] = Bones[BlendIndexArray.x*3+2] * BoneWeight.x;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[3] = vec4(0.0,0.0,0.0,1.0);");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[0] += Bones[BlendIndexArray.y*3] * BoneWeight.y;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[1] += Bones[BlendIndexArray.y*3+1] * BoneWeight.y;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[2] += Bones[BlendIndexArray.y*3+2] * BoneWeight.y;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[0] += Bones[BlendIndexArray.z*3] * BoneWeight.z;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[1] += Bones[BlendIndexArray.z*3+1] * BoneWeight.z;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tBoneToLocal[2] += Bones[BlendIndexArray.z*3+2] * BoneWeight.z;");
        strcat((char *)&vtxbuf, s);
        if ( (flags & 0x100) != 0 )
        {
            snprintf(s, 0x200uLL, "\tBoneToLocal[0] += Bones[BlendIndexArray.w*3] * BoneWeight.w;");
            strcat((char *)&vtxbuf, s);
            snprintf(s, 0x200uLL, "\tBoneToLocal[1] += Bones[BlendIndexArray.w*3+1] * BoneWeight.w;");
            strcat((char *)&vtxbuf, s);
            snprintf(s, 0x200uLL, "\tBoneToLocal[2] += Bones[BlendIndexArray.w*3+2] * BoneWeight.w;");
            strcat((char *)&vtxbuf, s);
        }
        snprintf(s, 0x200uLL, "\tvec4 WorldPos = ObjMatrix * (vec4(Position,1.0) * BoneToLocal);");
    }
    else
    {
        snprintf(s, 0x200uLL, "\tvec4 WorldPos = ObjMatrix * vec4(Position,1.0);");
    }
    strcat((char *)&vtxbuf, s);
    if ( (flags & 0x800000) != 0 )
    {
        snprintf(s, 0x200uLL, "    vec3 ReflVector = WorldPos.xyz - CameraPosition.xyz;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tvec3 ReflPos = normalize(ReflVector);");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "    ReflPos.xy = normalize(ReflPos.xy) * (ReflPos.z * 0.5 + 0.5);");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tgl_Position = vec4(ReflPos.xy, length(ReflVector) * 0.002, 1.0);");
    }
    else
    {
        snprintf(s, 0x200uLL, "\tvec4 ViewPos = ViewMatrix * WorldPos;");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "\tgl_Position = ProjMatrix * ViewPos;");
    }
    strcat((char *)&vtxbuf, s);
    if ( (flags & 2) != 0 )
    {
        if ( (flags & 0x201) == 513 && (flags & 0xE000) != 0 )
        {
            snprintf(s, 0x200uLL, "vec3 WorldNormal = normalize(vec3(WorldPos.xy - CameraPosition.xy, 0.0001)) * 0.85;");
        }
        else if ( (flags & 0x180) != 0 )
        {
            snprintf(s, 0x200uLL, "vec3 WorldNormal = mat3(ObjMatrix) * (Normal * mat3(BoneToLocal));");
        }
        else
        {
            snprintf(s, 0x200uLL, "vec3 WorldNormal = (ObjMatrix * vec4(Normal,0.0)).xyz;");
        }
        goto LABEL_57;
    }
    if ( flags & 0x1000040 )
    {
        snprintf(s, 0x200uLL, "vec3 WorldNormal = vec3(0.0, 0.0, 0.0);");
        LABEL_57:
        strcat((char *)&vtxbuf, s);
    }
    if ( (flags & 0x400) != 0 )
    {
        snprintf(
                s,
                0x200uLL,
                "Out_FogAmt = clamp((length(WorldPos.xyz - CameraPosition.xyz) - FogDistances.x) * FogDistances.z, 0.0, 0.90);");
        strcat((char *)&vtxbuf, s);
    }
    if ( (flags & 0x20) != 0 )
    {
        if ( (flags & 0x40000) != 0 )
        {
            snprintf(v9, 0x100uLL, "TexCoord0.xy / TexCoord0.w");
        }
        else if ( (flags & 0x20000) != 0 )
        {
            snprintf(v9, 0x100uLL, "TexCoord0 / 512.0");
        }
        else
        {
            snprintf(v9, 0x100uLL, "TexCoord0");
        }
        if ( (flags & 0x2000000) != 0 )
            snprintf(s, 0x200uLL, "Out_Tex0 = (NormalMatrix * vec3(%s, 1.0)).xy;", v9);
        else
            snprintf(s, 0x200uLL, "Out_Tex0 = %s;", v9);
        strcat((char *)&vtxbuf, s);
    }
    if ( flags & 0x1000040 )
    {
        snprintf(s, 0x200uLL, "vec3 reflVector = normalize(WorldPos.xyz - CameraPosition.xyz);");
        strcat((char *)&vtxbuf, s);
        snprintf(s, 0x200uLL, "reflVector = reflVector - 2.0 * dot(reflVector, WorldNormal) * WorldNormal;");
        strcat((char *)&vtxbuf, s);
        if ( (flags & 0x1000000) != 0 )
            snprintf(s, 0x200uLL, "Out_Refl = reflVector;");
        else
            snprintf(s, 0x200uLL, "Out_Tex1 = vec2(length(reflVector.xy), (reflVector.z * 0.5) + 0.25);");
        strcat((char *)&vtxbuf, s);
    }
    if ( (flags & 0x100000) != 0 )
    {
        snprintf(s, 0x200uLL, "lowp vec4 InterpColor = mix(GlobalColor, Color2, ColorInterp);");
        strcat((char *)&vtxbuf, s);
        v4 = "InterpColor";
    }
    else
    {
        v4 = "GlobalColor";
    }
    if ( (flags & 2) != 0 )
    {
        snprintf(v9, 0x200uLL, "vec3 Out_LightingColor;");
        strcat((char *)&vtxbuf, v9);
        if ( (flags & 8) != 0 )
        {
            if ( (flags & 0x200) != 0 )
                snprintf(v9, 0x200uLL, "Out_LightingColor = AmbientLightColor * MaterialAmbient.xyz * 1.5;");
            else
                snprintf(v9, 0x200uLL, "Out_LightingColor = AmbientLightColor * MaterialAmbient.xyz + %s.xyz;", s);
        }
        else
        {
            snprintf(v9, 0x200uLL, "Out_LightingColor = AmbientLightColor * MaterialAmbient.xyz + MaterialEmissive.xyz;");
        }
        strcat((char *)&vtxbuf, v9);
        if ( (flags & 0xE000) != 0 )
        {
            if ( (flags & 0x2000) != 0 )
            {
                if ( (flags & 0x1180) != 0 && CHook::CallFunction<int>("_Z22GetMobileEffectSettingv") == 3 )
                    snprintf(
                            v9,
                            0x200uLL,
                            "Out_LightingColor += (max(dot(DirLightDirection, WorldNormal), 0.0) + max(dot(DirBackLightDirection, WorldNo"
                            "rmal), 0.0)) * DirLightDiffuseColor;");
                else
                    snprintf(
                            v9,
                            0x200uLL,
                            "Out_LightingColor += max(dot(DirLightDirection, WorldNormal), 0.0) * DirLightDiffuseColor;");
                strcat((char *)&vtxbuf, v9);
                if ( (flags & 0x4000) == 0 )
                {
                    LABEL_88:
                    if ( (flags & 0x8000) == 0 )
                        goto LABEL_90;
                    goto LABEL_89;
                }
            }
            else if ( (flags & 0x4000) == 0 )
            {
                goto LABEL_88;
            }
            snprintf(
                    v9,
                    0x200uLL,
                    "Out_LightingColor += max(dot(DirLight2Direction, WorldNormal), 0.0) * DirLight2DiffuseColor;");
            strcat((char *)&vtxbuf, v9);
            if ( (flags & 0x8000) != 0 )
            {
                LABEL_89:
                snprintf(
                        v9,
                        0x200uLL,
                        "Out_LightingColor += max(dot(DirLight3Direction, WorldNormal), 0.0) * DirLight3DiffuseColor;");
                strcat((char *)&vtxbuf, v9);
            }
        }
    }
    LABEL_90:
    if ( (flags & 0x12) != 0 )
    {
        if ( (flags & 2) != 0 )
        {
            if ( (flags & 0x10) != 0 )
                snprintf(
                        v9,
                        0x200uLL,
                        "Out_Color = vec4((Out_LightingColor.xyz + %s.xyz * 1.5) * MaterialDiffuse.xyz, (MaterialAmbient.w) * %s.w);",
                        s,
                        s);
            else
                snprintf(
                        v9,
                        0x200uLL,
                        "Out_Color = vec4(Out_LightingColor * MaterialDiffuse.xyz, MaterialAmbient.w * %s.w);",
                        s);
            strcat((char *)&vtxbuf, v9);
            snprintf(v9, 0x200uLL, "Out_Color = clamp(Out_Color, 0.0, 1.0);");
        }
        else
        {
            snprintf(v9, 0x200uLL, "Out_Color = %s;", s);
        }
        strcat((char *)&vtxbuf, v9);
    }
    if ( !*(uint8_t *)(g_libGTASA + (VER_x32 ? 0x6B8BA4:0x896138)) && (flags & 0x2000) != 0 )
    {
        float v7 = 10.0;
        if ( *(uint8_t *)(g_libGTASA + (VER_x32 ? 0x6B8BA9:0x89613D)) )
            v7 = 9.0;
        snprintf(
                v9,
                0x200uLL,
                "float specAmt = max(pow(dot(reflVector, DirLightDirection), %.1f), 0.0) * EnvMapCoefficient * 2.0;",
                v7);
        strcat((char *)&vtxbuf, v9);
        snprintf(v9, 0x200uLL, "Out_Spec = specAmt * DirLightDiffuseColor;");
        strcat((char *)&vtxbuf, v9);
    }
    LABEL_111:
    if ( (flags & 0x80000) != 0 )
    {
        snprintf(v9, 0x200uLL, "Out_WaterDetail = (Out_Tex0 * 4.0) + vec2(WaterSpecs.x * -0.3, WaterSpecs.x * 0.21);");
        strcat((char *)&vtxbuf, v9);
        snprintf(v9, 0x200uLL, "Out_WaterDetail2 = (Out_Tex0 * -8.0) + vec2(WaterSpecs.x * 0.12, WaterSpecs.x * -0.05);");
        strcat((char *)&vtxbuf, v9);
        snprintf(v9, 0x200uLL, "Out_WaterAlphaBlend = distance(WorldPos.xy, CameraPosition.xy) * WaterSpecs.y;");
        strcat((char *)&vtxbuf, v9);
    }
    snprintf(s, 0x200uLL, "}");
    strcat((char *)&vtxbuf, s);
}

void BuildPixelSource(int flags) {
    char s[512]; // [xsp+8h] [xbp-248h] BYREF

    snprintf(s, 0x200uLL, "precision mediump float;");
    strcat((char *)&pxlbuf, s);
    if ( (flags & 0x20) != 0 )
    {
        snprintf(s, 0x200uLL, "uniform sampler2D Diffuse;");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "varying mediump vec2 Out_Tex0;");
        strcat((char *)&pxlbuf, s);
    }
    if ( (flags & 0x1000040) != 0 )
    {
        snprintf(s, 0x200uLL, "uniform sampler2D EnvMap;");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "uniform lowp float EnvMapCoefficient;");
        strcat((char *)&pxlbuf, s);
        if ( (flags & 0x40) != 0 )
            snprintf(s, 0x200uLL, "varying mediump vec2 Out_Tex1;");
        else
            snprintf(s, 0x200uLL, "varying mediump vec3 Out_Refl;");
    }
    else
    {
        if ( (flags & 0x10000) == 0 )
            goto LABEL_10;
        snprintf(s, 0x200uLL, "uniform sampler2D EnvMap;");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "uniform float DetailTiling;");
    }
    strcat((char *)&pxlbuf, s);
    LABEL_10:
    if ( (flags & 0x400) != 0 )
    {
        snprintf(s, 0x200uLL, "varying mediump float Out_FogAmt;");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "uniform lowp vec3 FogColor;");
        strcat((char *)&pxlbuf, s);
    }
    if ( (flags & 0x12) != 0 )
    {
        snprintf(s, 0x200uLL, "varying lowp vec4 Out_Color;");
        strcat((char *)&pxlbuf, s);
    }
    if ( (flags & 0x2000) != 0 && (flags & 0x1000040) == 1 )
    {
        snprintf(s, 0x200uLL, "varying lowp vec3 Out_Spec;");
        strcat((char *)&pxlbuf, s);
    }
    if ( (flags & 4) != 0 )
    {
        snprintf(s, 0x200uLL, "uniform lowp float AlphaModulate;");
        strcat((char *)&pxlbuf, s);
    }
    if ( (flags & 0x80000) != 0 )
    {
        snprintf(s, 0x200uLL, "varying mediump vec2 Out_WaterDetail;");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "varying mediump vec2 Out_WaterDetail2;");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "varying mediump float Out_WaterAlphaBlend;");
        strcat((char *)&pxlbuf, s);
    }
    snprintf(s, 0x200uLL, "void main()");
    strcat((char *)&pxlbuf, s);
    snprintf(s, 0x200uLL, "{");
    strcat((char *)&pxlbuf, s);
    snprintf(s, 0x200uLL, "lowp vec4 fcolor;");
    strcat((char *)&pxlbuf, s);
    if ( (flags & 0x20) == 0 )
    {
        if ( (flags & 0x12) != 0 )
            snprintf(s, 0x200uLL, "fcolor = Out_Color;");
        else
            snprintf(s, 0x200uLL, "fcolor = 0.0;");
        goto LABEL_40;
    }
    if ( (flags & 0x800) != 0 )
    {
        snprintf(s, 0x200uLL, "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -1.5);");
    }
    else
    {
        snprintf(s, 0x200uLL, "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -0.5);");
    }
    strcat((char *)&pxlbuf, s);
    snprintf(s, 0x200uLL, "fcolor = diffuseColor;");
    strcat((char *)&pxlbuf, s);
    if ( (flags & 0x12) != 0 )
    {
        if ( (flags & 0x10000) == 0 )
        {
            snprintf(s, 0x200uLL, "fcolor *= Out_Color;");
            strcat((char *)&pxlbuf, s);
            goto LABEL_33;
        }
        if ( (flags & 0x80000) != 0 )
        {
            snprintf(
                    s,
                    0x200uLL,
                    "float waterDetail = texture2D(EnvMap, Out_WaterDetail, -1.0).x + texture2D(EnvMap, Out_WaterDetail2, -1.0).x;");
            strcat((char *)&pxlbuf, s);
            snprintf(s, 0x200uLL, "fcolor *= vec4(Out_Color.xyz * waterDetail * 1.1, Out_Color.w);");
            strcat((char *)&pxlbuf, s);
            goto LABEL_38;
        }
        snprintf(
                s,
                0x200uLL,
                "fcolor *= vec4(Out_Color.xyz * texture2D(EnvMap, Out_Tex0.xy * DetailTiling, -0.5).xyz * 2.0, Out_Color.w);");
        LABEL_40:
        strcat((char *)&pxlbuf, s);
        if ( (flags & 0x40) == 0 )
            goto LABEL_42;
        goto LABEL_41;
    }
    LABEL_33:
    if ( (flags & 0x80000) != 0 )
    {
        LABEL_38:
        snprintf(s, 0x200uLL, "fcolor.a += Out_WaterAlphaBlend;");
        goto LABEL_40;
    }
    if ( (flags & 0x40) == 0 )
        goto LABEL_42;
    LABEL_41:
    snprintf(s, 0x200uLL, "fcolor.xyz = mix(fcolor.xyz, texture2D(EnvMap, Out_Tex1).xyz, EnvMapCoefficient);");
    strcat((char *)&pxlbuf, s);
    LABEL_42:
    if ( (flags & 0x1000000) != 0 )
    {
        snprintf(s, 0x200uLL, "vec2 ReflPos = normalize(Out_Refl.xy) * (Out_Refl.z * 0.5 + 0.5);");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "ReflPos = (ReflPos * vec2(0.5,0.5)) + vec2(0.5,0.5);");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "lowp vec4 ReflTexture =  texture2D(EnvMap, ReflPos);");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "fcolor.xyz = mix(fcolor.xyz,ReflTexture.xyz, EnvMapCoefficient);");
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "fcolor.w += ReflTexture.b * 0.125;");
        strcat((char *)&pxlbuf, s);
    }
    if ( (flags & 0x1000040) == 1 && (flags & 0x2000) != 0 && !*(uint8_t *)(g_libGTASA + (VER_x32 ? 0x6B8BA4:0x896138)) )
    {
        snprintf(s, 0x200uLL, "fcolor.xyz += Out_Spec;");
        strcat((char *)&pxlbuf, s);
    }
    if ( (flags & 0x400) != 0 )
    {
        snprintf(s, 0x200uLL, "fcolor.xyz = mix(fcolor.xyz, FogColor, Out_FogAmt);");
        strcat((char *)&pxlbuf, s);
    }
    if ( (flags & 0x4000000) != 0 )
    {
        snprintf(s, 0x200uLL, "fcolor.xyz = pow(fcolor.xyz, vec3(1.0 / 2.2));");
        strcat((char *)&pxlbuf, s);
    }
    snprintf(s, 0x200uLL, "gl_FragColor = fcolor;");
    strcat((char *)&pxlbuf, s);
    if ( (flags & 1) != 0 )
    {
        snprintf(s, 0x200uLL, "/*ATBEGIN*/");
        strcat((char *)&pxlbuf, s);
        if ( (flags & 0x800) != 0 )
        {
            snprintf(s, 0x200uLL, "if (diffuseColor.a < 0.8) { discard; }");
        }
        else if ( (flags & 0x200) != 0 )
        {
            snprintf(s, 0x200uLL, "gl_FragColor.a = Out_Color.a;");
            strcat((char *)&pxlbuf, s);
            snprintf(s, 0x200uLL, "if (diffuseColor.a < 0.5) { discard; }");
        }
        else
        {
            snprintf(s, 0x200uLL, "if (diffuseColor.a < 0.2) { discard; }");
        }
        strcat((char *)&pxlbuf, s);
        snprintf(s, 0x200uLL, "/*ATEND*/");
        strcat((char *)&pxlbuf, s);
    }
    if ( (flags & 4) != 0 )
    {
        snprintf(s, 0x200uLL, "gl_FragColor.a *= AlphaModulate;");
        strcat((char *)&pxlbuf, s);
    }
    snprintf(s, 0x200uLL, "}");
    strcat((char *)&pxlbuf, s);
}

int RQShader__BuildSource(int flags, char **pxlsrc, char **vtxsrc) {
    pxlbuf[0] = '\0';
    vtxbuf[0] = '\0';

    BuildPixelSource(flags);
    BuildVertexSource(flags);

    *pxlsrc = pxlbuf;
    *vtxsrc = vtxbuf;

    return 1;
}


void SetUpGLHooks()
{
    CHook::Redirect("_ZN8RQShader11BuildSourceEjPPKcS2_", RQShader__BuildSource);
}