#pragma once
#include "Vector3.h"
#include "Matrix4.h"

class Quaternion
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
public:
	Quaternion() {};
	Quaternion(float w, float x, float y, float z) :w(w), x(x), y(y), z(z) {};
	void SetQuaternion(Vector3 v, float angle);
	Quaternion SetQuaternion(Vector3 v);

	const Vector3& GetVec() { return Vector3(x, y, z); }
	void SetVec(const Vector3& vec);

	Quaternion CalcProduct(const Quaternion& q, const Quaternion& r);
	Quaternion GetIdentity();
	Quaternion GetConjugation();
	void Normalize();

	Matrix4 GetRotMatrix();

	Quaternion& operator/=(float s);
};

// Ï
Quaternion Multiply(const Quaternion& q1, const Quaternion& q2);

// ’PˆÊ
Quaternion IndentityQuaternion();

// ‹¤–ğ
Quaternion Conjugate(const Quaternion& q);

// norm
float Norm(const Quaternion& q);

// ³‹K‰»
Quaternion Normalize(const Quaternion& q);

// ‹t
Quaternion Inverce(const Quaternion& q);

// ”CˆÓ²‰ñ“]
Quaternion MakeAxisAngle(const Vector3& axis, float angle);

// ‰ñ“]‚ÌŒ‹‰Ê
Vector3 RotateVector(const Vector3& vec, const Quaternion& q);

// ‰ñ“]s—ñ
Matrix4 MakeRotateMatrix(const Quaternion& q);