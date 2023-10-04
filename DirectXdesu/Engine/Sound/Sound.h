#pragma once
#include "KDirectXCommon.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
//#include <combaseapi.h>

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
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	// ������
	void Init();

	/// <summary>
	/// ���ǂݍ���
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	SoundData SoundLoadWave(const char* filename);

	/// <summary>
	/// ���폜
	/// </summary>
	/// <param name="soundData"></param>
	void SoundUnLoad(SoundData* soundData);

	/// <summary>
	/// �Đ�
	/// </summary>
	/// <param name="soundData"></param>
	void SoundPlayWave(const SoundData& soundData);

	// �Q�b�^�[
	ComPtr<IXAudio2> GetxAudio() { return xAudio2; }

	// �C���X�^���X
	static Sound* GetInstance();

private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

private:
	Sound() = default;
	~Sound() = default;

	Sound(const Sound&) = default;
	const Sound& operator=(const Sound&) = delete;
};

