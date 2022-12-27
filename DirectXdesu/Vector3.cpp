#include "Vector3.h"
#include <cmath>

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector3::Length()const {
	return std::sqrt((x * x) + (y * y) + (z * z));
}

Vector3& Vector3::Normalize() {
	float len = Length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

float Vector3::Dot(const Vector3& v)const {
	return (x * v.x) + (y * v.y) + (z * v.z);
}

Vector3 Vector3::Cross(const Vector3& v)const {
	Vector3 vec3;
	vec3.x = y * v.z - v.y * z;
	vec3.y = z * v.x - x * v.z;
	vec3.z = x * v.y - v.x * y;
	return vec3;
}

Vector3 Vector3::operator+()const {
	return *this;
}

Vector3 Vector3::operator-()const {
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v) {
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp /= s;
}