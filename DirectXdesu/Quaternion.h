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
	void SetQuaternion(KMyMath::Vector3 v, float angle);
	Quaternion SetQuaternion(KMyMath::Vector3 v);

	const KMyMath::Vector3& GetVec() { return KMyMath::Vector3(x, y, z); }
	void SetVec(const KMyMath::Vector3& vec);

	Quaternion CalcProduct(const Quaternion& q, const Quaternion& r);
	Quaternion GetIdentity();
	Quaternion GetConjugation();
	void Normalize();

	KMyMath::Matrix4 GetRotMatrix();

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
Quaternion MakeAxisAngle(const KMyMath::Vector3& axis, float angle);

// ��]�̌���
KMyMath::Vector3 RotateVector(const KMyMath::Vector3& vec, const Quaternion& q);

// ��]�s��
KMyMath::Matrix4 MakeRotateMatrix(const Quaternion& q);

float Dot(const Quaternion& q1, const Quaternion& q2);

Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float time);

Quaternion DirectionToDirection(const KMyMath::Vector3& v1, const KMyMath::Vector3& v2);