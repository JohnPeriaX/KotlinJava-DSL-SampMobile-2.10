#include "../main.h"
#include "../game/common.h"
#include "../game/util.h"

#include "MicroIcon.h"

#include "PluginConfig.h"

bool MicroIcon::Init() noexcept
{
    if(MicroIcon::initStatus)
        return false;

    try
    {
        MicroIcon::tPassiveIcon = (RwTexture*)CUtil::LoadTextureFromDB("samp", "voicepassive");
        MicroIcon::tActiveIcon = (RwTexture*)CUtil::LoadTextureFromDB("samp", "voiceactive");
        MicroIcon::tMutedIcon = (RwTexture*)CUtil::LoadTextureFromDB("samp", "voicemuted");
    }
    catch(const std::exception& exception)
    {
        LogVoice("[sv:err:microicon:init] : failed to create icons");
        MicroIcon::tPassiveIcon = nullptr;
        MicroIcon::tActiveIcon = nullptr;
        MicroIcon::tMutedIcon = nullptr;
        return false;
    }

    if(!PluginConfig::IsMicroLoaded())
    {
        PluginConfig::SetMicroLoaded(true);
    }

    MicroIcon::initStatus = true;

    return true;
}

void MicroIcon::Free() noexcept
{
    if(!MicroIcon::initStatus)
        return;

    MicroIcon::tPassiveIcon = nullptr;
    MicroIcon::tActiveIcon = nullptr;
    MicroIcon::tMutedIcon = nullptr;

    MicroIcon::initStatus = false;
}

void MicroIcon::Show() noexcept
{
    MicroIcon::hasShowed = true;
    MicroIcon::showStatus = true;
}

bool MicroIcon::IsShowed() noexcept
{
    return MicroIcon::showStatus;
}

void MicroIcon::Hide() noexcept
{
    MicroIcon::showStatus = false;
}

bool MicroIcon::initStatus { false };
bool MicroIcon::showStatus { false };
bool MicroIcon::hasShowed { false };

RwTexture* MicroIcon::tPassiveIcon { nullptr };
RwTexture* MicroIcon::tActiveIcon { nullptr };
RwTexture* MicroIcon::tMutedIcon { nullptr };
