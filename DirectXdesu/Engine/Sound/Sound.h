#pragma once
#include "KDirectXCommon.h"

#pragma warning(push)
#pragma warning(disable: 4668)
#include <xaudio2.h>
#include <fstream>
#pragma warning(pop)

#pragma comment(lib,"xaudio2.lib")

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

struct SoundData
{
	BYTE* pBuffer;
	unsigned int bufferSize;
	WAVEFORMATEX wfex;
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

class Sound
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	// 初期化
	void Init();

	/// <summary>
	/// 音読み込み
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	SoundData SoundLoadWave(const char* filename);

	/// <summary>
	/// 音削除
	/// </summary>
	/// <param name="soundData"></param>
	void SoundUnLoad(SoundData* soundData);

	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="soundData"></param>
	void SoundPlayWave(const SoundData& soundData);

	// ゲッター
	ComPtr<IXAudio2> GetxAudio();

	// インスタンス
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

