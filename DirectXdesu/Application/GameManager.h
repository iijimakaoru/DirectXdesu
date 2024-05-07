#pragma once
#include <map>
#include <string>

class GameManager {
public:
	static GameManager* GetInstance();

private:
	GameManager() = default;
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	const GameManager& operator=(const GameManager&) = delete;

public:
	void Init();

	void Update();

	const bool GetIsStartMovie() const;

	void SetIsStartMovie(bool isStartMovie_);

	const bool& GetMovieFlag(std::string flagName);

	void SetMovieFlag(bool movieFlag, std::string flagName);

private:
	bool isStartMovie = false;

	std::map<std::string, bool> movieFlag_;
};
