#pragma once
#include "Sound.h"

class BGMManager {
public:
	/// <summary>
	/// インスタンス
	/// </summary>
	/// <returns></returns>
	static BGMManager* GetInstance();

private:
	BGMManager() = default;
	~BGMManager() = default;
	BGMManager(const BGMManager&) = default;
	const BGMManager& operator=(const BGMManager&) = delete;

public:
	void Init();

	void Finalize();

	void LoadBGM();

	void SoundPlay(SoundData bgmData);

	void SoundStop(SoundData bgmData);

	SoundData GetBGM1();

private:
	Sound* sound;

	SoundData gameBGM1;
};
