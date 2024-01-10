#include "Vector2.h"
#include <cmath>
#include <math.h>

using namespace MesiEngine;

using namespace KMyMath;

	// ゼロベクトル
	Vector2::Vector2() :x(0), y(0) {}

	Vector2::Vector2(float x, float y) : x(x), y(y) {}

	Vector2::~Vector2() {}

	// ベクトルの長さ
	float Vector2::Length()const {
		return sqrtf(LengthSq());
	}

	float Vector2::LengthSq() const
	{
		return this->x * this->x + this->y * this->y;
	}

	// 正規化
	Vector2& Vector2::normalize() {
		float length = this->Length();
		if (length != 0) {
			this->x = this->x / length;
			this->y = this->y / length;
		}
		else {
			this->x = 0;
			this->y = 0;
		}
		return *this;
	}

	float Vector2::dot(const Vector2& v) const {
		return (x * v.x) + (y * v.y);
	}

	float Vector2::cross(const Vector2& v) const {
		return (x * v.y) - (y * v.x);
	}

	Vector2 Vector2::operator+()const {
		return *this;
	}

	Vector2 Vector2::operator-()const {
		return Vector2(-x, -y);
	}

	Vector2& Vector2::operator+=(const Vector2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2& Vector2::operator*=(float s) {
		x *= s;
		y *= s;
		return *this;
	}

	Vector2& Vector2::operator/=(float s) {
		x /= s;
		y /= s;
		return *this;
	}

	const Vector2 operator+(const Vector2& v1, const Vector2& v2) {
		Vector2 temp(v1);
		return temp += v2;
	}

	const Vector2 operator-(const Vector2& v1, const Vector2& v2) {
		Vector2 temp(v1);
		return temp -= v2;
	}

	const Vector2 operator*(const Vector2& v, float s) {
		Vector2 temp(v);
		return temp *= s;
	}

	const Vector2 operator*(float s, const Vector2& v) {
		return v * s;
	}

	const Vector2 operator/(const Vector2& v, float s) {
		Vector2 temp(v);
		return temp /= s;
	}
}