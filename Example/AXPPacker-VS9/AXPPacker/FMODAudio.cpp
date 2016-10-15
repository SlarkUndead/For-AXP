
#include "stdafx.h"
#include <fmod_errors.h>
#include "FMODAudio.h"
#include <fmod.hpp>

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

FMOD::System* g_pAudioSys = 0;		// FMOD 音频系统
FMOD::ChannelGroup* g_pSndGrp = 0;	// 声音实例全放在这个组里


bool g_fmod_loop = false;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool IsFmodEnable()
{
	return (g_pAudioSys && g_pSndGrp);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 没有错误返回 TRUE, 否则 FALSE
bool ERRCHECK(FMOD_RESULT result)
{
    if(result == FMOD_OK)	return true;
	UILogWriter(CLR_TXT, "FMOD Audio System ERROR : [%d]<%s>", result, FMOD_ErrorString(result));
	return false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 创建FMOD音频系统
bool NtfCreateFMODAudioSystem(int nChannel)
{
	UILogWriter(CLR_TXT, "Creating FMOD Audio System ...");

	FMOD_RESULT result = FMOD_OK;

	result = FMOD::System_Create(&g_pAudioSys);
	if(!ERRCHECK(result))
	{
		g_pAudioSys = 0;
		UILogWriter(CLR_TXT, "CAN NOT Create FMOD Audio System");
		return false;
	}

	unsigned int version = 0;
	result = g_pAudioSys->getVersion(&version);
    if (!ERRCHECK(result))
	{
		UILogWriter(CLR_TXT, "FMOD Error(1)");
		NtfShutDownFMod();
		return false;
	}
	if (version < FMOD_VERSION)
    {
		UILogWriter(CLR_TXT, "FMOD Error : You are using an old version of FMOD %08x. This program requires 0x%08x", version, FMOD_VERSION);
		NtfShutDownFMod();
        return false;
    }

	int numdrivers = 0;
	result = g_pAudioSys->getNumDrivers(&numdrivers);
    if (!ERRCHECK(result))
	{
		UILogWriter(CLR_TXT, "FMOD Error(2)");
		NtfShutDownFMod();
		return false;
	}

	if (numdrivers == 0)
	{
		result = g_pAudioSys->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		if (!ERRCHECK(result))
		{
			UILogWriter(CLR_TXT, "FMOD Error(3)");
			NtfShutDownFMod();
			return false;
		}
	}
	else
	{
		FMOD_SPEAKERMODE speakermode;
		FMOD_CAPS caps;
		result = g_pAudioSys->getDriverCaps(0, &caps, 0, &speakermode);
		if (!ERRCHECK(result))
		{
			UILogWriter(CLR_TXT, "FMOD Error(4)");
			NtfShutDownFMod();
			return false;
		}

		result = g_pAudioSys->setSpeakerMode(speakermode);
		if (!ERRCHECK(result))
		{
			UILogWriter(CLR_TXT, "FMOD Error(5)");
			NtfShutDownFMod();
			return false;
		}

		if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			result = g_pAudioSys->setDSPBufferSize(1024, 10);
			if (!ERRCHECK(result))
			{
				UILogWriter(CLR_TXT, "FMOD Error(6)");
				NtfShutDownFMod();
				return false;
			}
		}

		char name[256];
		result = g_pAudioSys->getDriverInfo(0, name, 256, 0);
		if (!ERRCHECK(result))
		{
			UILogWriter(CLR_TXT, "FMOD Error(7)");
			NtfShutDownFMod();
			return false;
		}

		if(strstr(name, "SigmaTel"))
		{
			result = g_pAudioSys->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			if (!ERRCHECK(result))
			{
				UILogWriter(CLR_TXT, "FMOD Error(8)");
				NtfShutDownFMod();
				return false;
			}
		}
	}

    result = g_pAudioSys->init(nChannel, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		result = g_pAudioSys->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		if (!ERRCHECK(result))
		{
			UILogWriter(CLR_TXT, "FMOD Error(9)");
			NtfShutDownFMod();
			return false;
		}
	    result = g_pAudioSys->init(nChannel, FMOD_INIT_NORMAL, 0);
	}

    if (!ERRCHECK(result))
	{
		UILogWriter(CLR_TXT, "FMOD Error(10)");
		NtfShutDownFMod();
		return false;
	}

	result = g_pAudioSys->createChannelGroup("sndfx", &g_pSndGrp);
	if(!ERRCHECK(result))
	{
		g_pSndGrp = 0;
		UILogWriter(CLR_TXT, "failed to create group of SoundEffect");
		NtfShutDownFMod();
		return false;
	}

	g_pSndGrp->setVolume(1.5f);
	UILogWriter(CLR_TXT, "FMOD Audio System Created. FMOD Version = 0x%08x, Channels = %d", version, nChannel);

	//int num2d, num3d, total;
	int numXd[] = {0, 0, 0};
	result = g_pAudioSys->getHardwareChannels(numXd);//(&num2d, &num3d, &total);
	if (ERRCHECK(result))
	{
		//UILogWriter(CLR_TXT, "FMOD HardwareChannels %d, %d, %d", num2d, num3d, total);
		UILogWriter(CLR_TXT, "FMOD HardwareChannels %d, %d, %d", numXd[0], numXd[1], numXd[2]);
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 关闭FMOD音频系统
void NtfShutDownFMod()
{
	if(g_pAudioSys)
	{
		//UILogWriter(CLR_TXT, "Shutting FMOD Audio System ...");
		if(g_pSndGrp)
		{
			g_pSndGrp->stop();
			g_pSndGrp->release();
		}
		g_pSndGrp = 0;
		g_pAudioSys->release();
		g_pAudioSys = 0;
		//UILogWriter(CLR_TXT, "FMOD Audio System ShutDown");
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 只清理内存, 系统依然有效
void NtfClearAudioSystem()
{
	if(g_pAudioSys && g_pSndGrp)
	{
		g_pSndGrp->stop();
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// Update
void NtfUpdateAudioSystem(float dt)
{
	if(g_pAudioSys) g_pAudioSys->update();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 播放声音, 返回声音长度
float NtfPlay2DSoundEffect(const char* szNm, const char* pBf, size_t dwLen)
{
	if(!IsFmodEnable()) return 0.0f;
	g_pSndGrp->stop();
	if(szNm && pBf && (dwLen > 0))
	{
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = dwLen;

		FMOD::Sound* pSndBf = 0;
		FMOD_MODE flags = FMOD_OPENMEMORY | FMOD_2D;
		if(g_fmod_loop) flags |= FMOD_LOOP_NORMAL;
		else  flags |= FMOD_LOOP_OFF;
		FMOD_RESULT result = g_pAudioSys->createSound(pBf, flags, &exinfo, &pSndBf);
		if(ERRCHECK(result))
		{
			unsigned int dwLen = 0;
			pSndBf->setLoopCount(g_fmod_loop ? -1 : 1);
			pSndBf->getLength(&dwLen, FMOD_TIMEUNIT_MS);
			float fTm = 0.001f * dwLen;


			FMOD::Channel* pChannel = 0;
			FMOD_RESULT result = g_pAudioSys->playSound(FMOD_CHANNEL_FREE, pSndBf, true, &pChannel);
			if(ERRCHECK(result))
			{
				pChannel->setChannelGroup(g_pSndGrp);
				pChannel->setLoopCount(1);
				pChannel->setVolume(1.5f);
				pChannel->setPaused(false);
			}
			return fTm;
		}
	}
	return 0.0;
}
