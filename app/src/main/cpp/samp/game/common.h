#pragma once

// ==================================================================================
// CONFIGURATION & INCLUDES
// ==================================================================================

#include <cstdint>
#include <cstdio>
#include <string>
#include <span>
#include <array>
#include <vector>
#include <cassert>
#include <chrono>
#include <cmath>
#include <algorithm>

#include <android/log.h> // Keep Android Log support

// Game Headers (Re-enable if you have the gta-reversed folder structure)
#include "Vector.h" // Assumed common vector class
#include "game/Enums/eWeaponType.h" // Assumed

// Version & Features
#define VER_SAMP false
#define VER_MULTIPLAYER true
#define USE_FILE_LOG true
#define VER_2_1 true
#define USE_CUTSCENE_SHADOW_FOR_PED

// ==================================================================================
// BASIC TYPES & MACROS
// ==================================================================================

// Basic Types
typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;
typedef intptr_t  intptr;
typedef uintptr_t uintptr;

typedef uint8     bool8;
typedef uint16    bool16;
typedef uint32    bool32;
typedef unsigned char byte;

// IDs
typedef uint16 VEHICLEID;
typedef uint16 PLAYERID;
typedef uint16 ACTORID;

// Helper Macros
#define ASSERT assert
#define nil nullptr
#define UNUSED(x) (void)(x);
#define PADDING(x,y) uint8_t x[y]
#define SQR(x) ((x) * (x))

#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)

// Bitwise Macros
#define BIT_SET(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define BIT_CLEAR(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define BIT_CHECK(byte,nbit) ((byte) &   (1<<(nbit)))

// State Check Macros
#define IN_VEHICLE(x) ((x->dwStateFlags & 0x100) >> 8)
#define IS_CROUCHING(x) ((x->dwStateFlags >> 26) & 1)
#define IS_FIRING(x) (x->bFiringWeapon)
#define IS_TARGETING(x) (x->bIsAimingGun)

// Debug / Verification
namespace notsa {
    namespace detail {
        static void VerifyMacroImpl(bool result) {
            assert(result); 
        }
    };
};
#define VERIFY notsa::detail::VerifyMacroImpl
#define VERIFY_TODO_FIX(_expr) (_expr)

// Profiling Macros
#define START_PROFILE    auto start_time = std::chrono::steady_clock::now();
#define END_PROFILE auto end_time = std::chrono::steady_clock::now(); \
auto elapsed_time_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); \
auto elapsed_time_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time); \
__android_log_print(ANDROID_LOG_INFO, "PROFILE", "Time micro sec = %lld, milli = %lld", elapsed_time_microseconds.count(), elapsed_time_milliseconds.count());

// ==================================================================================
// MATH CONSTANTS & UTILS
// ==================================================================================

constexpr float E              = 2.71828f;          // e
constexpr float E_CONST        = 0.577f;            // Euler-Mascheroni constant
constexpr float PI             = 3.14159f;          // π
constexpr float HALF_PI        = PI / 2.0f;         // π / 2
constexpr float TWO_PI         = 6.28318f;          // τ (TAU)
constexpr float SQRT_2         = 1.41421f;          // √2
constexpr float SQRT_3         = 1.73205f;          // √3
constexpr float COS_45         = SQRT_2;            // cos(45deg)
// ... (Include other math constants if needed, truncated for brevity)

// Math Functions
constexpr float sq(float x) { return x * x; }

template <typename T>
T clamp(T value, T low, T high) {
    return std::min(std::max(value, low), high);
}

template<typename T>
auto lerp(const T& from, const T& to, float t) {
    return to * t + from * (1.f - t);
}

inline const float invLerp(float fMin, float fMax, float fVal) {
    return (fVal - fMin) / (fMax - fMin);
}

inline bool approxEqual(float f1, float f2, float epsilon) {
    return std::fabs(f1 - f2) < epsilon;
}

// Conversions
#define DEGTORAD(x) ((x) * PI / 180.0f)
#define RADTODEG(x) ((x) * 180.0f / PI)

constexpr float RadiansToDegrees(float angleInRadians) { return angleInRadians * 180.0F / PI; }
constexpr float DegreesToRadians(float angleInDegrees) { return angleInDegrees * PI / 180.0F; }

// ==================================================================================
// GAME CONSTANTS (LIMITS & IDs)
// ==================================================================================

#define MAX_PLAYERS         1004
#define MAX_VEHICLES        2000
#define MAX_PLAYER_NAME     24
#define PLAYER_PED_SLOTS    210
#define MAX_WEAPON_MODELS   47
#define MAX_WEAPONS_SLOT    13
#define MAX_MATERIALS       16

// HUD & Map
#define HUD_ELEMENT_MAP         1
#define HUD_ELEMENT_TAGS        2
#define HUD_ELEMENT_BUTTONS     3
#define HUD_ELEMENT_HUD         4
#define HUD_ELEMENT_VOICE       5
#define HUD_ELEMENT_TEXTLABELS  6
#define HUD_ELEMENT_FPS         7
#define HUD_MAX                 8

#define MAPICON_LOCAL               0
#define MAPICON_GLOBAL              1
#define MAPICON_LOCAL_CHECKPOINT    2
#define MAPICON_GLOBAL_CHECKPOINT   3

// Vehicle Subtypes
#define VEHICLE_SUBTYPE_CAR         1
#define VEHICLE_SUBTYPE_BIKE        2
#define VEHICLE_SUBTYPE_HELI        3
#define VEHICLE_SUBTYPE_BOAT        4
#define VEHICLE_SUBTYPE_PLANE       5
#define VEHICLE_SUBTYPE_PUSHBIKE    6
#define VEHICLE_SUBTYPE_TRAIN       7

// Special Vehicles
#define TRAIN_PASSENGER_LOCO        538
#define TRAIN_FREIGHT_LOCO          537
#define TRAIN_PASSENGER             570
#define TRAIN_FREIGHT               569
#define TRAIN_TRAM                  449
#define HYDRA                       520

// Actions
#define ACTION_WASTED               55
#define ACTION_DEATH                54
#define ACTION_INCAR                50
#define ACTION_NORMAL               1
#define ACTION_SCOPE                12
#define ACTION_NONE                 0 

// Screen & View
constexpr auto DEFAULT_SCREEN_WIDTH       = 640;
constexpr auto DEFAULT_SCREEN_HEIGHT      = 448;
#define SCREEN_WIDTH ((float)RsGlobal->maximumWidth)
#define SCREEN_HEIGHT ((float)RsGlobal->maximumHeight)
inline float SCREEN_STRETCH_X(float a)            { return a * SCREEN_WIDTH  / (float)DEFAULT_SCREEN_WIDTH; }
inline float SCREEN_STRETCH_Y(float a)            { return a * SCREEN_HEIGHT / (float)DEFAULT_SCREEN_HEIGHT; }

// ==================================================================================
// ENUMS
// ==================================================================================

enum eLights : uint8_t
{
    LEFT_HEADLIGHT = 0,
    RIGHT_HEADLIGHT,
    LEFT_TAIL_LIGHT,
    RIGHT_TAIL_LIGHT,
    MAX_LIGHTS            // MUST BE 16 OR LESS
};

enum eDoors
{
    BONNET = 0,
    BOOT,
    FRONT_LEFT_DOOR,
    FRONT_RIGHT_DOOR,
    REAR_LEFT_DOOR,
    REAR_RIGHT_DOOR,
    MAX_DOORS
};

enum eDoorStatus
{
    DT_DOOR_INTACT = 0,
    DT_DOOR_SWINGING_FREE,
    DT_DOOR_BASHED,
    DT_DOOR_BASHED_AND_SWINGING_FREE,
    DT_DOOR_MISSING
};

enum eWheelPosition
{
    FRONT_LEFT_WHEEL = 0,
    REAR_LEFT_WHEEL,
    FRONT_RIGHT_WHEEL,
    REAR_RIGHT_WHEEL,
    MAX_WHEELS
};

enum ePanels
{
    FRONT_LEFT_PANEL = 0,
    FRONT_RIGHT_PANEL,
    REAR_LEFT_PANEL,
    REAR_RIGHT_PANEL,
    WINDSCREEN_PANEL,
    FRONT_BUMPER,
    REAR_BUMPER,
    MAX_PANELS            // MUST BE 8 OR LESS
};

enum eComponentStatus
{
    DT_PANEL_INTACT = 0,
    DT_PANEL_BASHED,
    DT_PANEL_BASHED2,
    DT_PANEL_MISSING
};

enum eWeaponState : uint32 {
    WEAPONSTATE_READY = 0,
    WEAPONSTATE_FIRING,
    WEAPONSTATE_RELOADING,
    WEAPONSTATE_OUT_OF_AMMO,
    WEAPONSTATE_MELEE_MADECONTACT,
};

enum eSpecialObject {
    OBJECT_CELLPHONE = 330,
    OBJECT_CJ_CIGGY = 1485,
    OBJECT_DYN_BEER_1 = 1486,
    OBJECT_CJ_BEER_B_2 = 1543,
    OBJECT_CJ_PINT_GLASS = 1546,
    OBJECT_PARACHUTE = 3131,
    OBJECT_NOMODELFILE = 18631
};

// ==================================================================================
// GAME STRUCTURES
// ==================================================================================

// Animation Data
#pragma pack(push, 1)
typedef struct _ANIMATION_DATA {
    union {
        int iValue;
        struct {
            unsigned short sId : 16;
            unsigned char cFrameDelta : 8;
            unsigned char cLoopA : 1;
            unsigned char cLoopX : 1;
            unsigned char cLoopY : 1;
            unsigned char cLoopF : 1;
            unsigned char cTime : 2;
        };
    };
} ANIMATION_DATA;
#pragma pack(pop)
VALIDATE_SIZE(ANIMATION_DATA, 4);

// Damage Manager
#pragma pack(push, 1)
typedef struct _DAMAGE_MANAGER_INTERFACE
{
    float    fWheelDamageEffect;
    uint8_t  bEngineStatus;
    uint8_t  Wheel[MAX_WHEELS];
    uint8_t  Door[MAX_DOORS];
    uint32_t Lights;            // 2 bits per light
    uint32_t Panels;            // 4 bits per panel
} DAMAGE_MANAGER_INTERFACE;
#pragma pack(pop)
//VALIDATE_SIZE(DAMAGE_MANAGER_INTERFACE, 24);

// Weapon Structure
struct CWeapon
{
    eWeaponType dwType;
    eWeaponState dwState;
    union {
        uint32_t dwAmmoInClip;
        uint32_t m_nAmmoInClip;
    };
    uint32_t dwAmmo;
    uint32_t m_nTimer;
    bool m_bFirstPersonWeaponModeSelected;
    bool m_bDontPlaceInHand;
    uint8_t pad[2];
    uintptr_t *m_pWeaponFxSys;
};
static_assert(sizeof(CWeapon) == (VER_x32 ? 0x1C : 0x20), "Invalid size CWeapon");

// Audio Properties
struct VehicleAudioPropertiesStruct
{
    int16_t VehicleType;
    int16_t EngineOnSound;
    int16_t EngineOffSound;
    int16_t field_4;
    float field_5;
    float field_6;
    char HornTon;
    char field_8[3];
    float HornHigh;
    char DoorSound;
    char field_11[1];
    char RadioNum;
    char RadioType;
    char field_14;
    char field_15[3];
    float field_16;
};
//VALIDATE_SIZE(VehicleAudioPropertiesStruct, 24);

// Bullet Sync
struct BULLET_SYNC
{
    uint8_t hitType;
    uint16_t hitId;
    float origin[3];
    float hitPos[3];
    float offsets[3];
    uint8_t weapId;
};

// Resource Entry
typedef struct _RES_ENTRY_OBJ
{
    PADDING(_pad0, 48);     // 0-48
    uintptr_t validate;     //48-52
    PADDING(_pad1, 4);      //52-56
} RES_ENTRY_OBJ;
//VALIDATE_SIZE(RES_ENTRY_OBJ, 60);

// ==================================================================================
// WEAPON MODELS
// ==================================================================================

#define WEAPON_MODEL_BRASSKNUCKLE       331
#define WEAPON_MODEL_GOLFCLUB           333
#define WEAPON_MODEL_NITESTICK          334
#define WEAPON_MODEL_KNIFE              335
#define WEAPON_MODEL_BAT                336
#define WEAPON_MODEL_SHOVEL             337
#define WEAPON_MODEL_POOLSTICK          338
#define WEAPON_MODEL_KATANA             339
#define WEAPON_MODEL_CHAINSAW           341
#define WEAPON_MODEL_DILDO              321
#define WEAPON_MODEL_DILDO2             322
#define WEAPON_MODEL_VIBRATOR           323
#define WEAPON_MODEL_VIBRATOR2          324
#define WEAPON_MODEL_FLOWER             325
#define WEAPON_MODEL_CANE               326
#define WEAPON_MODEL_GRENADE            342
#define WEAPON_MODEL_TEARGAS            343
#define WEAPON_MODEL_MOLOTOV            344
#define WEAPON_MODEL_COLT45             346
#define WEAPON_MODEL_SILENCED           347
#define WEAPON_MODEL_DEAGLE             348
#define WEAPON_MODEL_SHOTGUN            349
#define WEAPON_MODEL_SAWEDOFF           350
#define WEAPON_MODEL_SHOTGSPA           351
#define WEAPON_MODEL_UZI                352
#define WEAPON_MODEL_MP5                353
#define WEAPON_MODEL_AK47               355
#define WEAPON_MODEL_M4                 356
#define WEAPON_MODEL_TEC9               372
#define WEAPON_MODEL_RIFLE              357
#define WEAPON_MODEL_SNIPER             358
#define WEAPON_MODEL_ROCKETLAUNCHER     359
#define WEAPON_MODEL_HEATSEEKER         360
#define WEAPON_MODEL_FLAMETHROWER       361
#define WEAPON_MODEL_MINIGUN            362
#define WEAPON_MODEL_SATCHEL            363
#define WEAPON_MODEL_BOMB               364
#define WEAPON_MODEL_SPRAYCAN           365
#define WEAPON_MODEL_FIREEXTINGUISHER   366
#define WEAPON_MODEL_CAMERA             367
#define WEAPON_MODEL_NIGHTVISION        368
#define WEAPON_MODEL_INFRARED           369
#define WEAPON_MODEL_JETPACK            370
#define WEAPON_MODEL_PARACHUTE          371

// Helpers
inline void LittleTest() {
    ((void(__cdecl *)())(g_libGTASA + (VER_x32 ? 0x3FCAF0 + 1 : 0x4E0224)))();
}