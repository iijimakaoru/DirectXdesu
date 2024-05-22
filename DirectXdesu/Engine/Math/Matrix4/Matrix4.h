#pragma once
#include "Vector3.h"
#include <array>

/**
 * @file Matrix4.h
 * @brief 行列
 * @author 飯島 薫
 */

namespace KMyMath {
class Matrix4 {
public:
	std::array<std::array<float, 4>, 4> m;

public:
	// コンストラクタ
	Matrix4();
	Matrix4(
	    float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
	    float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

	Vector3 transform(const Vector3& v, const Matrix4& m1);

	// 代入演算子
	Matrix4& operator=(const Matrix4& m_);

	// 加算
	Matrix4& operator+=(const Matrix4& m_);
	// 減算
	Matrix4& operator-=(const Matrix4& m_);
	// 乗算
	Matrix4& operator*=(const Matrix4& m_);

	// 加算
	Matrix4 operator+(const Matrix4& m_) const;
	// 減算
	Matrix4 operator-(const Matrix4& m_) const;
	// 乗算
	Matrix4 operator*(const Matrix4& m_) const;
};

const Vector3 operator*(const Vector3& v, const Matrix4& m2);
} // namespace KMyMath