#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "Matrix4.h"
#include "Vector3.h"

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
	// 乱数
	int GetRand(int min, int max);
	float GetRand(float min, float max);
	double GetRand(double min, float max);

	// 視点座標、注視座標、上方ベクトル
	Matrix LockAt(const Vector3& eye, const Vector3& target, const Vector3& up);

	Matrix PerspectiveFov(float fovY, float nearZ, float farZ);
	Matrix Ortho(float nearZ, float farZ);

	float ConvertToRad(float angle);
};

