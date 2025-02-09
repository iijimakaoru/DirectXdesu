#include "Timer.h"
#include <sstream>

Timer::Timer()
{
	totalTime = 0;
	deltaTime = 1;
}

Timer::~Timer()
{
}

float Timer::GetDeltaTime() const
{
	return deltaTime;
}

float Timer::GetTotalTime() const
{
	return totalTime;
}

void Timer::UpdateTimer()
{
	totalTime += deltaTime;
}