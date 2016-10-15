
#pragma once

namespace FMOD
{
	class System;
    class Sound;
    class Channel;
    class ChannelGroup;
    class SoundGroup;
};


bool NtfCreateFMODAudioSystem(int nChannel);								// 创建FMOD音频系统
void NtfShutDownFMod();														// 关闭FMOD音频系统
void NtfClearAudioSystem();													// 只清理内存, 系统依然有效
void NtfUpdateAudioSystem(float dt);										// Update
float NtfPlay2DSoundEffect(const char* szNm, const char* pBf, size_t dwLen);// 播放声音, 返回声音长度