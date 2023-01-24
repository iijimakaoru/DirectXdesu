#include "Vector4.h"
#include <cmath>

Vector4::Vector4() : x(0), y(0), z(0), w(0) {

}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {

}

Vector4::Vector4(Vector3 v3d, float w) : x(v3d.x), y(v3d.y), z(v3d.z), w(w)
{
}

float Vector4::Length() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

Vector4& Vector4::Normalize()
{
	float len = Length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

Vector4 Vector4::operator+() const
{
	return *this;
}

Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

Vector4& Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4& Vector4::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

Vector4& Vector4::operator*=(const Matrix& m)
{
	Vector4 ans;
	ans.x = x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0];
	ans.y = x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1];
	ans.z = x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2];
	ans.w = x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3];
	*this = ans;
	return *this;
}

const Vector4 operator-(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);
	return temp -= v2;
}
