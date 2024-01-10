#include "Vector4.h"
#include <cmath>

using namespace MesiEngine;
using namespace KMyMath;

Vector4::Vector4() : x(0), y(0), z(0), w(0) {}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vector4::Vector4(KMyMath::Vector3 v3d, float w) : x(v3d.x), y(v3d.y), z(v3d.z), w(w) {}

float Vector4::Length() const { return sqrt(x * x + y * y + z * z + w * w); }

Vector4& Vector4::Normalize() {
	float len = Length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

float Vector4::dot(const Vector4& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

Vector4 Vector4::cross(const Vector4& v) const {
	Vector3 v1 = {x, y, z};
	Vector3 v2 = {v.x, v.y, v.z};
	Vector3 temp = v1.Cross(v2);
	return {temp.x, temp.y, temp.z, 0.0f};
}

Vector4 Vector4::operator+() const { return *this; }

Vector4 Vector4::operator-() const { return Vector4(-x, -y, -z, -w); }

Vector4& Vector4::operator+=(const Vector4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

Vector4& Vector4::operator-=(const Vector4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4& Vector4::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

Vector4& Vector4::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;

	return *this;
}

const Vector4 operator+(const Vector4& v1, const Vector4& v2) {
	Vector4 temp(v1);
	return temp += v2;
}

const Vector4 operator-(const Vector4& v1, const Vector4& v2) {
	Vector4 temp(v1);
	return temp -= v2;
}

const Vector4 operator*(const Vector4& v, float s) {
	Vector4 temp(v);
	return temp *= s;
}

const Vector4 operator*(float s, const Vector4& v) { return v * s; }

const Vector4 operator/(const Vector4& v, float s) {
	Vector4 temp(v);
	return temp /= s;
}

Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t) {
	Vector4 temp;
	Vector4Lerp(src1, src2, t, temp);
	return temp;
}

void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest) {
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
	dest.w = src1.w + (src2.w - src1.w) * t;
}
