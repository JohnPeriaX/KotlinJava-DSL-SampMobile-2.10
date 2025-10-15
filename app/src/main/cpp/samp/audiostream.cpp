#include <memory>
#include <unistd.h>
#include "audiostream.h"
#include "log.h"
#include "game/game.h"
#include "../main.h"
#include "../vendor/bass/bass.h"

extern CGame* pGame;

HSTREAM bassStream = 0;
char g_szAudioStreamUrl[256+1];
float g_fAudioStreamX;
float g_fAudioStreamY;
float g_fAudioStreamZ;
float g_fAudioStreamRadius;
bool g_audioStreamUsePos;
bool g_bAudioStreamStop;
bool g_bAudioStreamThreadWorked;

void* audioStreamThread(void*)
{
	g_bAudioStreamThreadWorked = true;
	if (bassStream)
	{
		BASS_ChannelStop(bassStream);
		bassStream = 0;
	}

	bassStream = BASS_StreamCreateURL(g_szAudioStreamUrl, 0, 0x940000, 0,0);
	BASS_ChannelPlay(bassStream, 0);

	//BASS_ChannelSetSync(bassStream, 4, 0, 0, 0);
	//BASS_ChannelSetSync(bassStream, 12, 0, 0, 0);

	while (!g_bAudioStreamStop)
	{
		// ..
		usleep(2000);
	}

	BASS_ChannelStop(bassStream);
	bassStream = 0;
	g_bAudioStreamThreadWorked = false;
	pthread_exit(0);
}

CAudioStream::CAudioStream()
{
	m_bInited = false;
}

bool CAudioStream::Initialize()
{
	bassStream = 0;
	//BASS_Free();
	//if (!BASS_Init(-1, 44100, 0)) return false;		//if (!BASS_Init(-1, 48000, 0)) return false;

	BASS_SetConfigPtr(16, "SA-MP/0.3");
	//BASS_SetConfig(5, ) volume
	BASS_SetConfig(21, 1);			// BASS_CONFIG_NET_PLAYLIST
	BASS_SetConfig(11, 10000);		// BASS_CONFIG_NET_TIMEOUT

	m_bInited = true;
	return true;
}

void CAudioStream::Process()
{
	if (g_bAudioStreamThreadWorked)
	{
		// ..
		if (pGame->IsGamePaused())
		{
			BASS_SetConfig(5, 0);
		}
		else
		{
			BASS_SetConfig(5, 5000);
		}
	}
}

bool CAudioStream::Play(const char* szUrl, float fX, float fY, float fZ, float fRadius, bool bUsePos)
{
	FLog("Play: %s", szUrl);

	if (!m_bInited) return false;
	Stop(true);

	if (bassStream)
	{
		BASS_ChannelStop(bassStream);
		bassStream = 0;
	}

	memset(g_szAudioStreamUrl, 0, sizeof(g_szAudioStreamUrl));
	strncpy(g_szAudioStreamUrl, szUrl, 256);

	g_fAudioStreamX = fX;
	g_fAudioStreamY = fY;
	g_fAudioStreamZ = fZ;
	g_fAudioStreamRadius = fRadius;
	g_audioStreamUsePos = bUsePos;

	g_bAudioStreamStop = false;

	pthread_t thread;
	pthread_create(&thread, 0, audioStreamThread, 0);
	return true;
}
// 0.3.7
bool CAudioStream::Stop(bool bWaitThread)
{
	FLog("Stop: %s", g_szAudioStreamUrl);
	if (!m_bInited || !g_bAudioStreamThreadWorked)
		return false;

	g_bAudioStreamStop = true;

	if (bWaitThread)
	{
		while (g_bAudioStreamThreadWorked)
			usleep(200);
	}

	BASS_StreamFree(bassStream);
	bassStream = 0;
	return true;
}