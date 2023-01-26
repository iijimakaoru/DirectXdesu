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

	Matrix GetRotMatrix();

	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator*=(float s);
	Quaternion& operator/=(float s);
};

const Quaternion& operator+(const Quaternion& q1, const Quaternion& q2);
const Quaternion& operator*(const Quaternion& q1, float s);
const Quaternion& operator*(float s, const Quaternion& q1);

// ��
Quaternion Multiply(const Quaternion& q1, const Quaternion& q2);

// �P��
Quaternion IndentityQuaternion();

// ����
Quaternion Conjugate(const Quaternion& q);

// norm
float Norm(const Quaternion& q);

// ���K��
Quaternion Normalize(const Quaternion& q);

// �t
Quaternion Inverce(const Quaternion& q);

// �C�ӎ���]
Quaternion MakeAxisAngle(const Vector3& axis, float angle);

// ��]�̌���
Vector3 RotateVector(const Vector3& vec, const Quaternion& q);

// ��]�s��
Matrix MakeRotateMatrix(const Quaternion& q);

float Dot(const Quaternion& q1, const Quaternion& q2);

Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float time);