#pragma once

namespace KMyMath
{
	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		// �R���X�g���N�^
		// �[���x�N�g��
		Vector3();
		Vector3(float x, float y, float z);

		// �����o�֐�
		// �m����
		float Length()const;
		float LengthSq()const;
		// ���K��
		Vector3& Normalize();
		// ����
		float Dot(const Vector3& v)const;
		// �O��
		Vector3 Cross(const Vector3& v)const;

		// �P�����Z�q�I�[�o�[���[�h
		Vector3 operator+()const;
		Vector3 operator-()const;

		// ������Z�q�I�[�o�[���[�h
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
	};

	const Vector3 operator+(const Vector3& v1, const Vector3& v2);
	const Vector3 operator-(const Vector3& v1, const Vector3& v2);
	const Vector3 operator*(const Vector3& v1, const Vector3& v2);
	const Vector3 operator*(const Vector3& v, float s);
	const Vector3 operator*(float s, const Vector3& v);
	const Vector3 operator/(const Vector3& v, float s);
}