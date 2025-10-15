#pragma once

#include "vendor/bass/bass.h"

class CAudioStream
{
public:
	CAudioStream();

	bool Initialize();

	void Process();

	bool Play(const char* szUrl, float fX, float fY, float fZ, float fRadius, bool bUsePos);
	bool Stop(bool bWaitThread);

private:
	bool m_bInited;
};