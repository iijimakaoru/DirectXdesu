#pragma once
#include "Vector3.h"

namespace KMyMath
{
	class Vector4 {
	public:
		float x;
		float y;
		float z;
		float w;

		// �[���x�N�g��
		Vector4();
		Vector4(float x, float y, float z, float w);
		Vector4(KMyMath::Vector3 v3d, float w);

		// �m����
		float Length() const;
		// ���K��
		Vector4& Normalize();

		// ����
		float dot(const Vector4& v) const;

		// �O��
		Vector4 cross(const Vector4& v) const;

		// �P�����Z�q�I�[�o�[���[�h
		Vector4 operator+() const;
		Vector4 operator-() const;

		// ������Z�q�I�[�o�[���[�h
		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator/=(float s);
		Vector4& operator*=(float s);
	};

	// 2�����Z�q�I�[�o�[���[�h
	const Vector4 operator+(const Vector4& v1, const Vector4& v2);
	const Vector4 operator-(const Vector4& v1, const Vector4& v2);
	const Vector4 operator*(const Vector4& v, float s);
	const Vector4 operator*(float s, const Vector4& v);
	const Vector4 operator/(const Vector4& v, float s);

	void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest);

	Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t);
}