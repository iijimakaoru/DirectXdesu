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
	// ����
	int GetRand(int min, int max);
	float GetRand(float min, float max);
	double GetRand(double min, float max);

	// ���_���W�A�������W�A����x�N�g��
	KMyMath::Matrix4 LockAt(const KMyMath::Vector3& eye, const KMyMath::Vector3& target, const KMyMath::Vector3& up);

	KMyMath::Matrix4 PerspectiveFov(float fovY, float nearZ, float farZ);
	KMyMath::Matrix4 Ortho(float nearZ, float farZ);

	float ConvertToRad(float angle);
};

