#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"

namespace MyMathUtility
{
	// �x���烉�W�A���ѕϊ�����萔
	const float PI = 3.141592654f;
	constexpr float degree2Radius = 3.141592654f / 180.0f;
	constexpr float PIHalf = 3.141592654f / 2.0f;

	// �C�v�V����
	constexpr float EPSILON = 2.2204460492503131E-16f;
	
	/// <summary>
	/// ���������߂�(2d)
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Vector2Length(const KMyMath::Vector2& v);
	/// <summary>
	/// ���������߂�(3d)
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Vector3Length(const KMyMath::Vector3& v);

	/// <summary>
	/// Vector3���K��
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	KMyMath::Vector3 MakeNormalize(KMyMath::Vector3 v);

	// �P�ʍs��
	KMyMath::Matrix4 MakeIdentity();

	/// <summary>
	/// �g��s��
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeScaling(const KMyMath::Vector3& scale);

	/// <summary>
	/// ��]�s��
	/// </summary>
	/// <param name="rotation"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeRotation(const KMyMath::Vector3& rotation);

	/// <summary>
	/// ���s�s��
	/// </summary>
	/// <param name="trans"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeTranslation(const KMyMath::Vector3& trans);

	/// <summary>
	/// �T�C���ƃR�T�C��
	/// </summary>
	/// <param name="sin_"></param>
	/// <param name="cos_"></param>
	/// <param name="angle"></param>
	void SinCos(float& sin_, float& cos_, float angle);

	/// <summary>
	/// �x�N�g���ƍs��̊|���Z
	/// </summary>
	/// <param name="m"></param>
	/// <param name="v"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MatMulVector(KMyMath::Matrix4 m, KMyMath::Vector3 v);

	/// <summary>
	/// ���s���e�ϊ�(����)
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="top"></param>
	/// <param name="near_"></param>
	/// <param name="far_"></param>
	/// <param name="m"></param>
	void MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_, KMyMath::Matrix4 m);

	/// <summary>
	/// ���s���e�ϊ�(�E��)
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="top"></param>
	/// <param name="near_"></param>
	/// <param name="far_"></param>
	/// <param name="m"></param>
	void MakeOrthogonalR(float left, float right, float bottom, float top, float near_, float far_, KMyMath::Matrix4 m);

	/// <summary>
	/// �r���[�s��̍쐬
	/// </summary>
	/// <param name="eye"></param>
	/// <param name="target"></param>
	/// <param name="up"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeLockAt(KMyMath::Vector3& eye, KMyMath::Vector3& target, KMyMath::Vector3& up);

	/// <summary>
	/// �������e�쐬
	/// </summary>
	/// <param name="forAngleY"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="nearZ"></param>
	/// <param name="farZ"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakePerspective(float fogAngleY, float aspectRatio, float nearZ, float farZ);

	/// <summary>
	/// �t�s��
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeInverse(KMyMath::Matrix4& mat);

	// Sin,Cos,Tan�֐�
	float Sin(float sin);
	float Cos(float cos);
	float Tan(float tan);

	float Asin(float sin);
	float Acos(float cos);
	float Atan(float tan);
	float Atan2(float y, float x);

	void Complement(float& x1, float x2, float flame);

	float Clamp(float Value, const float low, const float high);
	float Clamp0To1(float val);

	/// <summary>
	/// �l����������
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	bool Approximately(float a, float b);

	KMyMath::Vector3 HermiteGetPoint(KMyMath::Vector3 p0, KMyMath::Vector3 p1, KMyMath::Vector3 v0, KMyMath::Vector3 v1, float t);
}

namespace KMyMath
{
	/// <summary>
	/// �x�N�g���ƍs��̊|���Z
	/// </summary>
	/// <param name="vec"></param>
	/// <param name="mat"></param>
	/// <returns></returns>
	Vector3 Vec3Mat4Mul(Vector3& vec, Matrix4& mat);

	/// <summary>
	/// �L���b�g���[���X�v���C���Ȑ�
	/// </summary>
	/// <param name="points"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatMullRomSprine(std::vector<Vector3>& points, float t);

	/// <summary>
	/// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	/// </summary>
	/// <param name="sep"></param>
	/// <param name="e1"></param>
	/// <param name="e2"></param>
	/// <param name="e3"></param>
	/// <returns></returns>
	float LenSegLineOfSeparateAxis(Vector3* sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);
}