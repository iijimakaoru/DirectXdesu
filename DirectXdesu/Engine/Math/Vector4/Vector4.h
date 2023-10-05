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

		// ゼロベクトル
		Vector4();
		Vector4(float x, float y, float z, float w);
		Vector4(KMyMath::Vector3 v3d, float w);

		// ノルム
		float Length() const;
		// 正規化
		Vector4& Normalize();

		// 内積
		float dot(const Vector4& v) const;

		// 外積
		Vector4 cross(const Vector4& v) const;

		// 単項演算子オーバーロード
		Vector4 operator+() const;
		Vector4 operator-() const;

		// 代入演算子オーバーロード
		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator/=(float s);
		Vector4& operator*=(float s);
	};

	// 2項演算子オーバーロード
	const Vector4 operator+(const Vector4& v1, const Vector4& v2);
	const Vector4 operator-(const Vector4& v1, const Vector4& v2);
	const Vector4 operator*(const Vector4& v, float s);
	const Vector4 operator*(float s, const Vector4& v);
	const Vector4 operator/(const Vector4& v, float s);

	void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest);

	Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t);
}