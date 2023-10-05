#pragma once

namespace KMyMath
{
	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		// コンストラクタ
		// ゼロベクトル
		Vector3();
		Vector3(float x, float y, float z);

		// メンバ関数
		// ノルム
		float Length()const;
		float LengthSq()const;
		// 正規化
		Vector3& Normalize();
		// 内積
		float Dot(const Vector3& v)const;
		// 外積
		Vector3 Cross(const Vector3& v)const;

		// 単項演算子オーバーロード
		Vector3 operator+()const;
		Vector3 operator-()const;

		// 代入演算子オーバーロード
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