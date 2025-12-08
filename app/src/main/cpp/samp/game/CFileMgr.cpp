//
// Created by x1y2z on 14.04.2023.
//

#include "CFileMgr.h"
#include "main.h"
#include "../vendor/shadowhook/patch.h"
#include <android/asset_manager.h>
#include <stdio.h>
#include <strings.h>

extern AAssetManager* g_pAssetManager;

static int asset_read(void* cookie, char* buf, int size) {
    return AAsset_read((AAsset*)cookie, buf, size);
}
static int asset_write(void* cookie, const char* buf, int size) {
    return -1; // Read only
}
static fpos_t asset_seek(void* cookie, fpos_t offset, int whence) {
    return AAsset_seek((AAsset*)cookie, offset, whence);
}
static int asset_close(void* cookie) {
    AAsset_close((AAsset*)cookie);
    return 0;
}

#include <ctype.h>
#include <string>
#include <algorithm>

static AAsset* OpenAssetCompat(const char* path) {
    // 1. Try exact match
    AAsset* asset = AAssetManager_open(g_pAssetManager, path, AASSET_MODE_UNKNOWN);
    if (asset) return asset;

    // 2. Try lowercase
    std::string lowerPath = path;
    std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::tolower);
    asset = AAssetManager_open(g_pAssetManager, lowerPath.c_str(), AASSET_MODE_UNKNOWN);
    if (asset) return asset;
    
    // 3. Try "Text" prefix normalization (common case "TEXT/..." -> "Text/...")
    if (strncasecmp(path, "TEXT/", 5) == 0) {
        std::string textPath = "Text/" + std::string(path + 5);
        // Try exact filename after Text/
        asset = AAssetManager_open(g_pAssetManager, textPath.c_str(), AASSET_MODE_UNKNOWN);
        if (asset) return asset;
        
        // Try lowercase filename after Text/
        std::string lowerFilename = path + 5;
        std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
        textPath = "Text/" + lowerFilename;
        asset = AAssetManager_open(g_pAssetManager, textPath.c_str(), AASSET_MODE_UNKNOWN);
        if (asset) return asset;
    }

    return nullptr;
}

void CFileMgr::SetDir(const char *path) {
    ( ( void(*)(const char *path) )(g_libGTASA + (VER_x32 ? 0x003F0C54 + 1 : 0x4D293C)) )(path);
}

FILE* CFileMgr::OpenFile(const char *path, const char *mode) {
    // Check for Text or fonts in assets (Case insensitive)
    if (g_pAssetManager && (strcasestr(path, "Text/") || strcasestr(path, "fonts/") || strcasestr(path, "scache") || strcasestr(path, "stream.ini") || strcasestr(path, "version.txt"))) {
        AAsset* asset = OpenAssetCompat(path);
        if (asset) {
            FILE* assetFile = funopen(asset, asset_read, asset_write, asset_seek, asset_close);
            if (assetFile) {
                // LOGI("Opened asset: %s", path);
                return assetFile;
            }
            AAsset_close(asset);
        }
    }

    sprintf(ms_path, "%s%s", g_pszStorage, path);

    auto file = fopen(ms_path, mode);

    if(!file) {
        FLog("Fail open file %s", ms_path);
    }
    return file;
}

int32_t CFileMgr::CloseFile(FILE* file) {
    return fclose(file);
}

void CFileMgr::Initialise() {
    CHook::CallFunction<void>("_ZN8CFileMgr10InitialiseEv"); // ������� ����� �����
}
