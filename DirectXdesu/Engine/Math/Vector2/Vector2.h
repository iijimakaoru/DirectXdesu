#pragma once

namespace KMyMath
{
	class Vector2
	{
	public:
		float x;
		float y;

		// �R���X�g���N�^
		Vector2();
		Vector2(float x, float y);
		~Vector2();

		// �����o�֐�
		float Length()const; // �m����
		float LengthSq()const;
		Vector2& normalize(); // ���K��
		float dot(const Vector2& v)const; // ����
		float cross(const Vector2& v)const; // �O��

		// �P�����Z�q�I�[�o�[���[�h
		Vector2 operator+() const;
		Vector2 operator-() const;

		// ������Z�q�I�[�o�[���[�h
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);
	};

	// 2�����Z�q
	const Vector2 operator+(const Vector2& v1, const Vector2& v2);
	const Vector2 operator-(const Vector2& v1, const Vector2& v2);
	const Vector2 operator*(const Vector2& v, float s);
	const Vector2 operator*(float s, const Vector2& v);
	const Vector2 operator/(const Vector2& v, float s);
}