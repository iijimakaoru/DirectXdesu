#pragma once
#include <array>

namespace KMyMath
{
	class Matrix4
	{
	public:
		std::array<std::array<float, 4>, 4> m;
	public:
		// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
		Matrix4();
		Matrix4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		
		// ‘ã“ü‰‰Zq
		Matrix4& operator=(const Matrix4& m_);

		// ‰ÁZ
		Matrix4& operator+=(const Matrix4& m_);
		// Œ¸Z
		Matrix4& operator-=(const Matrix4& m_);
		// æZ
		Matrix4& operator*=(const Matrix4& m_);

		// ‰ÁZ
		Matrix4 operator+(const Matrix4& m_) const;
		// Œ¸Z
		Matrix4 operator-(const Matrix4& m_) const;
		// æZ
		Matrix4 operator*(const Matrix4& m_) const;
	};
}