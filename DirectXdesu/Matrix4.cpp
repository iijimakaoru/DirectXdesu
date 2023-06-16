#include "Matrix4.h"
#include <cmath>
#include <stdexcept>

const float EPSILON = 0.000001f;

Matrix::Matrix()
{
	Identity();
}

void Matrix::Identity()
{
	for (size_t y = 0; y < 4; y++)
	{
		for (size_t x = 0; x < 4; x++)
		{
			if (x == y)
			{
				m[y][x] = 1.0f;
			}
			else
			{
				m[y][x] = 0.0f;
			}
		}
	}
}

Matrix& Matrix::operator*=(const Matrix& mat)
{
	Matrix tmp(*this);

	for (size_t y = 0; y < 4; y++)
	{
		for (size_t x = 0; x < 4; x++)
		{
			this->m[y][x] =
				(tmp.m[y][0] * mat.m[0][x]) +
				(tmp.m[y][1] * mat.m[1][x]) +
				(tmp.m[y][2] * mat.m[2][x]) +
				(tmp.m[y][3] * mat.m[3][x]);
		}
	}

	return *this;
}

Matrix Matrix::RotationX(float radian)
{
	Matrix mat;
	mat.m[1][1] = cosf(radian);
	mat.m[1][2] = sinf(radian);
	mat.m[2][1] = -sinf(radian);
	mat.m[2][2] = cosf(radian);
	return mat;
}

Matrix Matrix::RotationY(float radian)
{
	Matrix mat;
	mat.m[0][0] = cosf(radian);
	mat.m[0][2] = -sinf(radian);
	mat.m[2][0] = sinf(radian);
	mat.m[2][2] = cosf(radian);
	return mat;
}

Matrix Matrix::RotationZ(float radian)
{
	Matrix mat;
	mat.m[0][0] = cosf(radian);
	mat.m[0][1] = sinf(radian);
	mat.m[1][0] = -sinf(radian);
	mat.m[1][1] = cosf(radian);
	return mat;
}

Matrix Matrix::RotationAll(float radian)
{
	Matrix mat;
	mat *= RotationZ(radian);
	mat *= RotationX(radian);
	mat *= RotationY(radian);

	return mat;
}

KMyMath::Vector3 operator*(const KMyMath::Vector3 vec, const Matrix mat)
{
	KMyMath::Vector3 temp = vec;
	temp.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];
	temp.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];
	temp.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];
	return temp;
}

KMyMath::Vector3& operator*=(KMyMath::Vector3& vec, const Matrix mat)
{
	KMyMath::Vector3 temp = vec * mat;
	vec = temp;
	return vec;
}