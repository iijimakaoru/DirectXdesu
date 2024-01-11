#include "SEManager.h"

SEManager* SEManager::GetInstance() {
	static SEManager instance;
	return &instance;
}

void SEManager::Init() { sound = Sound::GetInstance(); }

void SEManager::Finalize() {
	sound->SoundUnLoad(&shotSE);
	sound->SoundUnLoad(&alertSE);
}

void SEManager::LoadSE() {
	shotSE = sound->SoundLoadWave("Resources/Sound/shotSE.wav");
	alertSE = sound->SoundLoadWave("Resources/Sound/alertSE.wav");
}

void SEManager::SoundPlay(SoundData seData) { sound->SoundPlayWave(seData, 0.4f); }

SoundData SEManager::GetShotSE() { return shotSE; }

SoundData SEManager::GetAlertSE() { return alertSE; }

SoundData SEManager::GetEShotSE() { return EshotSE; }

SoundData SEManager::GetBakuhaSE() { return bakuhaSE; }
