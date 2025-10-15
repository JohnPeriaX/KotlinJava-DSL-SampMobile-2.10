#pragma once

#include "../game/RW/RenderWare.h"

class CObject;
struct CVehicleGTA;
struct CPhysical;
struct CEntityGTA;
struct CPedGTA;
#include "../game/Core/Rect.h"

CPedGTA* GamePool_FindPlayerPed();
CPedGTA* GamePool_Ped_GetAt(int iID);
int GamePool_Ped_GetIndex(CPedGTA* pActor);
CPhysical *GamePool_Object_GetAt(int iID);
uintptr_t GamePool_Vehicle_GetIndex(CVehicleGTA* pGtaVehicle);
CVehicleGTA* GamePool_Vehicle_GetAt(int iID);

int GetVehicleSubtype(CVehicleGTA* pGtaVehicle);

uintptr_t GetModelInfoByID(int iModelID);
bool IsExistInfoForModel(int iModelID);
bool IsValidModel(int iModelID);
int GetModelRefCounts(int iModel);
bool IsValidPedModel(uint modelID);
uintptr_t GetModelRWObject(uint modelID);

//uintptr_t LoadTexture(const char* texname);

RwTexture* LoadTextureFromTxd(const char* txdname, const char* texturename);
int FindTxdSlot(const char* txdname);
void PushCurrentTxd();
void SetCurrentTxd(int slot);
void PopCurrentTxd();
uintptr_t SetTexture(const char* texturename);

float FloatOffset(float f1, float f2);
float GetDistance(RwV3d vec1, RwV3d vec2);
float DegToRad(float fDegress);
float fixAngle(float angle);
float subAngle(float a1, float a2);

void SetPlayerPedPtrRecord(uint8_t bytePlayer, CPedGTA* pPed);
uint8_t FindPlayerNumFromPedPtr(CPedGTA* dwPedPtr);

int GameGetWeaponModelIDFromWeaponID(int iWeaponID);

int GetFreeTextDrawTextureSlot();
void ResetTextDrawTextures();
void DestroyTextDrawTexture(int index);

void DrawTexture(uintptr_t texture, CRect* rect, uint32_t dwColor);
void DrawTextureUV(uintptr_t texture, CRect* rect, uint32_t dwColor, float* uv);
void DeleteRwTexture(uintptr_t texture);

bool IsPointInRect(float x, float y, CRect* rect);

RwObject* ModelInfoCreateInstance(int iModel);

void RenderClumpOrAtomic(uintptr_t rwObject);

float GetModelColSphereRadius(int iModel);

void GetModelColSphereVecCenter(int iModel, RwV3d* vec);

void DestroyAtomicOrClump(uintptr_t rwObject);

void GamePrepareTrain(CVehicleGTA* pGtaVehicle);

void DrawRaster(RwRaster* raster, CRect const& rect);

void GameResetStats();

void ProjectMatrix(CVector* vecOut, CMatrix* mat, CVector* vecPos);
void RwMatrixRotate(RwMatrix* mat, int axis, float angle);
void RwMatrixScale(RwMatrix* mat, RwV3d* vecScale);

const char* getGameDataFolderDirectory();

int LineOfSight(RwV3d* start, RwV3d* end, void* colpoint, uintptr_t ent, char buildings, char vehicles, char peds, char objects, char dummies, bool seeThrough, bool camera, bool unk);
void RwMatrixInvert(RwMatrix *matOut, RwMatrix *matIn);

int GetTaskTypeFromTask(uint32_t *task);
int Game_PedStatPrim(int model_id);

uint32_t GetUpperCaseKey(const char* szUpper);

RwMatrix* mat_invert(RwMatrix *dst, const RwMatrix *src);

const char* GetAnimByIdx(int idx);
int GetAnimIdxByName(const char* szName);

bool IsGameEntityArePlaceable(CEntityGTA *pEntity);

void RemoveBuilding(uint32_t dwModel, RwV3d vecPos, float fRange);
void RemoveObjectInRange(int iModel, RwV3d vecPos, float fRange);
void RemoveOccludersInRadius(RwV3d vecPos, float fRadius);

RwTexture* LoadTextureFromTxd(const char* txdname, const char* texturename);
uintptr_t LoadTexture(const char* texname);

RpAtomic* ObjectMaterialCallBack(RpAtomic* rpAtomic, CObject* pObject);
RpAtomic* ObjectMaterialTextCallBack(RpAtomic* rpAtomic, CObject* pObject);

bool GetAnimationIndexFromName(const char* szName);

void SetScissorRect(void* pRect);

bool IsValidGamePed(CPedGTA* pPed);
void DefinedState2d();
void DefinedState();
void RenderEntity(CEntityGTA* entity);