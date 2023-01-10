#pragma once
#include <string>
#include <vector>
#include <stdexcept>

#define PI 3.1415927f

class MyMath
{
public:
	static MyMath* GetInstance();

private:
	MyMath() = default;
	~MyMath() = default;
	MyMath(const MyMath&) = delete;
	const MyMath& operator=(const MyMath&) = delete;

public:
	// —”
	int GetRand(int min, int max);
	float GetRand(float min, float max);
	double GetRand(double min, float max);
};

