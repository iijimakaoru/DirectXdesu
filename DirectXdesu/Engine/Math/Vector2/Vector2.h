#pragma once

/**
 * @file Vector2.h
 * @brief 2D
 * @author 飯島 薫
 */

namespace KMyMath
{
	class Vector2
	{
	public:
		float x;
		float y;

		// コンストラクタ
		Vector2();
		Vector2(float x, float y);
		~Vector2();

		// メンバ関数
		float Length()const; // ノルム
		float LengthSq()const;
		Vector2& normalize(); // 正規化
		float dot(const Vector2& v)const; // 内積
		float cross(const Vector2& v)const; // 外積

		// 単項演算子オーバーロード
		Vector2 operator+() const;
		Vector2 operator-() const;

		// 代入演算子オーバーロード
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);
	};

	// 2項演算子
	const Vector2 operator+(const Vector2& v1, const Vector2& v2);
	const Vector2 operator-(const Vector2& v1, const Vector2& v2);
	const Vector2 operator*(const Vector2& v, float s);
	const Vector2 operator*(float s, const Vector2& v);
	const Vector2 operator/(const Vector2& v, float s);
}