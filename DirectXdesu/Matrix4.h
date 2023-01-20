#pragma once
#include "Vector3.h"
#include <DirectXMath.h>

class Matrix
{
public:
	float m[4][4];
public:
	Matrix();
	void Identity();
	Matrix& operator*=(const Matrix& m);

	static Matrix RotationX(float radian);
	static Matrix RotationY(float radian);
	static Matrix RotationZ(float radian);
	static Matrix RotationAll(float radian);
};

Vector3 operator*(const Vector3 vec, const Matrix mat);
Vector3& operator*=(Vector3& vec, const Matrix mat);