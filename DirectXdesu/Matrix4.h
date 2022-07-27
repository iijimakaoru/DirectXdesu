#pragma once
#include "Vector3.h"

struct Matrix4
{
	float m[4][4];
};

Matrix4 identity();

Matrix4 scale(const Vector3& s);

Matrix4 rotateX(float angle);
Matrix4 rotateY(float angle);
Matrix4 rotateZ(float angle);

Matrix4 translate(const Vector3& t);

Vector3 transform(const Vector3& v, const Matrix4& m);

Matrix4& operator *=(Matrix4& m1, Matrix4& m2);

const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m);