#include "SoundManager.h"

SoundManager* SoundManager::GetInstance() {
	static SoundManager instance;
	return &instance;
}
