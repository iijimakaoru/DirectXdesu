#include "Quaternion.h"
#include <cmath>

using namespace MesiEngine;

const float EPSILON = 0.000001f;

void Quaternion::SetQuaternion(KMyMath::Vector3 v, float angle)
{
	v.Normalize();

	float rad = sinf(angle / 2);
	x = v.x * rad;
	y = v.y * rad;
	z = v.z * rad;
	w = cosf(angle / 2);

	Normalize();
}

Quaternion Quaternion::SetQuaternion(KMyMath::Vector3 v)
{
	Quaternion r;
	r.x = v.x;
	r.y = v.y;
	r.z = v.z;
	r.w = 0;

	Quaternion q = *this;
	r = CalcProduct(q, r);
	r = CalcProduct(r, q.GetConjugation());

	return r;
}

void Quaternion::SetVec(const KMyMath::Vector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

Quaternion Quaternion::CalcProduct(const Quaternion& q, const Quaternion& r)
{
	Quaternion ans;
	ans.w = (q.w * r.w) - (q.x * r.x) - (q.y * r.y) - (q.z * r.z);
	ans.x = (q.y * r.z) - (q.z * r.y) + (r.w * q.x) + (q.w * r.x);
	ans.y = (q.z * r.x) - (q.x * r.z) + (r.w * q.y) + (q.w * r.y);
	ans.z = (q.x * r.y) - (q.y * r.x) + (r.w * q.z) + (q.w * r.z);

	return ans;
}

Quaternion Quaternion::GetIdentity()
{
	Quaternion ans;
	ans.x = 0;
	ans.y = 0;
	ans.z = 0;
	ans.w = 1;

	return ans;
}

Quaternion Quaternion::GetConjugation()
{
	Quaternion ans;
	ans.x = -x;
	ans.y = -y;
	ans.z = -z;
	ans.w = w;

	return ans;
}

void Quaternion::Normalize()
{
	float len = sqrtf((w * w) + (x * x) + (y * y) + (z * z));

	if (len != 0.0f)
	{
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}
}

KMyMath::Matrix4 Quaternion::GetRotMatrix()
{
	KMyMath::Matrix4 mat;
	//mat.m[0][0] = w * w + x * x - y * y - z * z;
	mat.m[0][0] = 1 - (2 * y * y) - (2 * z * z);
	mat.m[0][1] = (2 * x * y) + (2 * w * z);
	mat.m[0][2] = (2 * x * z) - (2 * w * y);

	mat.m[1][0] = (2 * x * y) - (2 * w * z);
	mat.m[1][1] = 1 - (2 * x * x) - (2 * z * z);
	mat.m[1][2] = (2 * y * z) + (2 * w * x);

	mat.m[2][0] = (2 * x * z) + (2 * w * y);
	mat.m[2][1] = (2 * y * z) - (2 * w * x);
	mat.m[2][2] = 1 - (2 * x * x) - (2 * y * y);
	//mat.m[2][2] = w * w - x * x - y * y + z * z;

	return mat;
}

Quaternion& Quaternion::operator+=(const Quaternion& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

Quaternion& Quaternion::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

Quaternion& Quaternion::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

const Quaternion& operator+(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion tmp(q1);
	return tmp += q2;
}

const Quaternion& operator*(const Quaternion& q1, float s)
{
	Quaternion tmp(q1);
	return tmp *= s;
}

const Quaternion& operator*(float s, const Quaternion& q1)
{
	return q1 * s;
}

Quaternion Multiply(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion ans;
	ans.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
	ans.x = (q1.y * q2.z) - (q1.z * q2.y) + (q2.w * q1.x) + (q1.w * q2.x);
	ans.y = (q1.z * q2.x) - (q1.x * q2.z) + (q2.w * q1.y) + (q1.w * q2.y);
	ans.z = (q1.x * q2.y) - (q1.y * q2.x) + (q2.w * q1.z) + (q1.w * q2.z);

	return ans;
}

Quaternion IndentityQuaternion()
{
	return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

Quaternion Conjugate(const Quaternion& q)
{
	Quaternion ans;
	ans.x = -q.x;
	ans.y = -q.y;
	ans.z = -q.z;
	ans.w = q.w;

	return ans;
}

float Norm(const Quaternion& q)
{
	return sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

Quaternion Normalize(const Quaternion& q)
{
	float len = Norm(q);

	Quaternion ans = q;
	if (len != 0.0f)
	{
		ans.x /= len;
		ans.y /= len;
		ans.z /= len;
		ans.w /= len;
	}

	return ans;
}

Quaternion Inverce(const Quaternion& q)
{
	Quaternion ans = q;
	float norm = Norm(ans);
	ans /= norm;
	ans /= norm;
	return ans;
}

Quaternion MakeAxisAngle(const KMyMath::Vector3& axis, float angle)
{
	Quaternion ans;
	KMyMath::Vector3 axis_ = axis;
	axis_.Normalize();

	float rad = sinf(angle / 2);
	ans.x = axis_.x * rad;
	ans.y = axis_.y * rad;
	ans.z = axis_.z * rad;
	ans.w = cosf(angle / 2);

	ans.Normalize();

	return ans;
}

KMyMath::Vector3 RotateVector(const KMyMath::Vector3& vec, const Quaternion& q)
{
	Quaternion vecQ;
	vecQ.SetVec(vec);
	Quaternion tmpQ = q;
	tmpQ.Normalize();
	Quaternion qConjugate = tmpQ.GetConjugation();
	vecQ = Multiply(q, vecQ);
	vecQ = Multiply(vecQ, qConjugate);

	return vecQ.GetVec();
}

KMyMath::Matrix4 MakeRotateMatrix(const Quaternion& q)
{
	KMyMath::Matrix4 mat;
	mat.m[0][0] = (q.w * q.w) + (q.x * q.x) - (q.y * q.y) - (q.z * q.z);
	mat.m[0][0] = 1 - (2 * q.y * q.y) - (2 * q.z * q.z);
	mat.m[0][1] = (2 * q.x * q.y) + (2 * q.w * q.z);
	mat.m[0][2] = (2 * q.x * q.z) - (2 * q.w * q.y);

	mat.m[1][0] = (2 * q.x * q.y) - (2 * q.w * q.z);
	mat.m[1][1] = 1 - (2 * q.x * q.x) - (2 * q.z * q.z);
	mat.m[1][2] = (2 * q.y * q.z) + (2 * q.w * q.x);

	mat.m[2][0] = (2 * q.x * q.z) + (2 * q.w * q.y);
	mat.m[2][1] = (2 * q.y * q.z) - (2 * q.w * q.x);
	mat.m[2][2] = 1 - (2 * q.x * q.x) - (2 * q.y * q.y);
	mat.m[2][2] = (q.w * q.w) - (q.x * q.x) - (q.y * q.y) + (q.z * q.z);

	return mat;
}

float Dot(const Quaternion& q1, const Quaternion& q2)
{
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float time)
{
	float dot = Dot(q1, q2);
	Quaternion tmp = q2;
	if (dot < 0)
	{
		tmp = q2;

		dot = -dot;
	}

	float scale0 = 1.0f - time;
	float scale1 = time;

	if (dot >= 1.0f - EPSILON)
	{
		return (1.0f - time) * q1 + time * tmp;
	}
	else
	{
		if ((1.0f - dot) > 0.001f)
		{
			float theta = (float)std::acos(dot);
			scale0 = (float)sin(theta + scale0) / sin(theta);
			scale1 = (float)sin(theta + scale1) / sin(theta);
		}

		return q1 * scale0 + tmp * scale1;
	}
}

Quaternion DirectionToDirection(const KMyMath::Vector3& v1, const KMyMath::Vector3& v2)
{
	// v1とv2を正規化して内積を求める。v1とv2を単位ベクトル前提とするなら正規化は不要
	float dot = v1.Dot(v2);
	// v1,v2の外積
	KMyMath::Vector3 cross = v1.Cross(v2);
	// 軸は単位ベクトルである必要があるので正規化
	// v1とv2が単位ベクトルであっても、外積が単位ベクトルとは限らないのでここの正規化は必須
	KMyMath::Vector3 axis = axis.Normalize();
	// 単位ベクトルで内積をとっているのでacosで角度を求める
	float theta = std::acos(dot);
	// axisとthetaで任意軸回転を作って返す
	MakeAxisAngle(axis, theta);

	return  MakeAxisAngle(axis, theta);
}
