#include "GameManager.h"

GameManager* GameManager::GetInstance() {
	static GameManager instance;
	return &instance;
}

const bool GameManager::GetIsStartMovie() const { return isStartMovie; }

void GameManager::SetIsStartMovie(bool isStartMovie_) { isStartMovie = isStartMovie_; }
