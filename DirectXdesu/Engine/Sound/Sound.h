#pragma once
#include "KDirectXCommon.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include <combaseapi.h>

struct ChunkHeader
{
	char id[4];
	int32_t size;
};

struct RiffHeader
{
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData
{
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	unsigned int bufferSize;
};

class Sound
{
public:
	void Init();
	SoundData SoundLoadWave(const char* filename);
	void SoundUnLoad(SoundData* soundData);
	void SoundPlayWave(const SoundData& soundData);
	ComPtr<IXAudio2> GetxAudio() { return xAudio2; }

private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
};

