#include "../../../main.h"

#include "KeyFilter.h"

bool KeyFilter::AddKey(const uint16_t keyId) noexcept
{
    if(KeyFilter::statusKeys[keyId])
        return false;

    LogVoice("[dbg:keyfilter] : adding key (id:0x%hhx)...", keyId);

    KeyFilter::statusKeys[keyId] = true;

    return true;
}

bool KeyFilter::RemoveKey(const uint16_t keyId) noexcept
{
    if(!KeyFilter::statusKeys[keyId])
        return false;

    LogVoice("[dbg:keyfilter] : removing key (id:0x%hhx)...", keyId);

    KeyFilter::statusKeys[keyId] = false;

    return true;
}

void KeyFilter::RemoveAllKeys() noexcept
{
    LogVoice("[dbg:keyfilter] : removing all keys...");

    for(uint16_t keyId { 0 }; keyId < KeyFilter::statusKeys.size(); ++keyId)
        KeyFilter::RemoveKey(keyId);
}

bool KeyFilter::IsKeyAvailable(const uint16_t keyId) noexcept
{
    if(!KeyFilter::statusKeys[keyId])
        return false;

    return true;
}

std::array<bool, 256> KeyFilter::statusKeys {};