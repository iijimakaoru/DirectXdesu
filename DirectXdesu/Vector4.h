#pragma once
#include "Vector3.h"
#include "Matrix4.h"

class Vector4{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(Vector3 v3d, float w);

	float Length() const;
	Vector4& Normalize();

	Vector4 operator+() const;

	Vector4 operator-() const;

	Vector4& operator-=(const Vector4& v);

	Vector4& operator/=(float s);

	Vector4& operator*=(const Matrix& m);
};

const Vector4 operator-(const Vector4& v1, const Vector4& v2);