#include "GameManager.h"

GameManager* GameManager::GetInstance() {
	static GameManager instance;
	return &instance;
}

void GameManager::Init() {
	movieFlag_["Start"] = false;
	movieFlag_["Boss"] = false;
	movieFlag_["Clear"] = false;
	movieFlag_["Over"] = false;
	movieFlag_["All"] = false;
}

void GameManager::Update() {
	if (movieFlag_["Start"] || movieFlag_["Boss"] || movieFlag_["Clear"]) {
		movieFlag_["All"] = true;
	} else {
		movieFlag_["All"] = false;
	}
}

const bool GameManager::GetIsStartMovie() const { return isStartMovie; }

void GameManager::SetIsStartMovie(bool isStartMovie_) { isStartMovie = isStartMovie_; }

const bool& GameManager::GetMovieFlag(std::string flagName) { return movieFlag_[flagName]; }

void GameManager::SetMovieFlag(bool movieFlag, std::string flagName) {
	movieFlag_[flagName] = movieFlag;
}
