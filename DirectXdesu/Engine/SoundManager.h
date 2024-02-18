#pragma once
class SoundManager {
public:
	static SoundManager* GetInstance();

private:
	SoundManager() = default;
	~SoundManager() = default;
	SoundManager(const SoundManager&) = delete;
	const SoundManager& operator=(const SoundManager&) = delete;

private:

};
