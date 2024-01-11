#pragma once
#include "Sound.h"

class SEManager {
public:
	/// <summary>
	/// インスタンス
	/// </summary>
	/// <returns></returns>
	static SEManager* GetInstance();

private:
	SEManager() = default;
	~SEManager() = default;
	SEManager(const SEManager&) = default;
	const SEManager& operator=(const SEManager&) = delete;

public:
	void Init();

	void Finalize();

	void LoadSE();

	void SoundPlay(SoundData seData);

	SoundData GetShotSE();

	SoundData GetAlertSE();

	SoundData GetEShotSE();

	SoundData GetBakuhaSE();

private:
	Sound* sound;

	SoundData shotSE;
	SoundData alertSE;
	SoundData EshotSE;
	SoundData bakuhaSE;
};
