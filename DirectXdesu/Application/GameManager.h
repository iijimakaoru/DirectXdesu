#pragma once
class GameManager {
public:
	static GameManager* GetInstance();

private:
	GameManager() = default;
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	const GameManager& operator=(const GameManager&) = delete;

public:
	const bool GetIsStartMovie() const;

	void SetIsStartMovie(bool isStartMovie_);

private:
	bool isStartMovie = false;
};
