#pragma once
#include <Windows.h>
#include <string>

class Timer
{
public:
	Timer();
	~Timer();

	float GetDeltaTime() const;
	float GetTotalTime() const;

	void UpdateTimer();

private:
	float totalTime;
	float deltaTime;
};

