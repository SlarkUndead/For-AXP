
#pragma once

namespace FMOD
{
	class System;
    class Sound;
    class Channel;
    class ChannelGroup;
    class SoundGroup;
};


bool NtfCreateFMODAudioSystem(int nChannel);								// ����FMOD��Ƶϵͳ
void NtfShutDownFMod();														// �ر�FMOD��Ƶϵͳ
void NtfClearAudioSystem();													// ֻ�����ڴ�, ϵͳ��Ȼ��Ч
void NtfUpdateAudioSystem(float dt);										// Update
float NtfPlay2DSoundEffect(const char* szNm, const char* pBf, size_t dwLen);// ��������, ������������