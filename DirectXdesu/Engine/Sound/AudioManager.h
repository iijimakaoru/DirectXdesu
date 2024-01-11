#pragma once
#include "KDirectXCommon.h"

#pragma warning(push)
#pragma warning(disable : 4668)
#include <fstream>
#include <xaudio2.h>
#pragma warning(pop)

#pragma comment(lib, "xaudio2.lib")

#include <map>

class AudioManager {
public:
	struct ChunkHeader {
		char id[4];
		int32_t size;
	};

	struct RiffHeader {
		ChunkHeader chunk;
		char type[4];
	};

	struct SoundData {
		BYTE* pBuffer;
		unsigned int bufferSize;
		WAVEFORMATEX wfex;
		IXAudio2SourceVoice* pSourceVoice;
	};

	struct FormatChunk {
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init(const std::string& directoryPath_ = "Resources/Sound/");

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// サウンド読み込み
	/// </summary>
	/// <param name="filename"></param>
	void SoundLoadWave(const std::string& fileName);

	/// <summary>
	/// サウンドデータの解放
	/// </summary>
	/// <param name="soundData"></param>
	void SoundUnLoad(SoundData* soundData);

	/// <summary>
	/// サウンド再生
	/// </summary>
	/// <param name="fileName"></param>
	void SoundPlayWave(std::string fileName, float volume = 0.25f, bool isLoop = false);

	void SoundStopWave(std::string fileName);

private:
	// XAudioのインスタンス
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;

	std::map<std::string, SoundData> soundDatas;

	std::string directoryPath;

public:
	static AudioManager* GetInstance();

private:
	AudioManager() = default;
	~AudioManager() = default;

	AudioManager(const AudioManager&) = default;
	const AudioManager& operator=(const AudioManager&) = delete;
};
