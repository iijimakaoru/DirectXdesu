#include "BGMManager.h"

BGMManager* BGMManager::GetInstance() {
	static BGMManager instance;
	return &instance;
}

void BGMManager::Init() { sound = Sound::GetInstance(); }

void BGMManager::Finalize() { sound->SoundUnLoad(&gameBGM1); }

void BGMManager::LoadBGM() { gameBGM1 = sound->SoundLoadWave("Resources/Sound/BattleBGM.wav"); }

void BGMManager::SoundPlay(SoundData bgmData) { sound->SoundPlayWave(bgmData, 0.4f, true); }

void BGMManager::SoundStop(SoundData bgmData) { sound->SoundStopWave(bgmData); }

SoundData BGMManager::GetBGM1() { return gameBGM1; }
