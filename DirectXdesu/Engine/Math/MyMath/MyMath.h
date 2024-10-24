#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <stdexcept>
#pragma warning(pop)

/**
 * @file MyMath.h
 * @brief 数学関数
 * @author 飯島 薫
 */

namespace MyMathUtility
{
	// 度からラジアンび変換する定数
	const float PI = 3.141592654f;
	const float PIDouble = 3.141592654f * 2.0f;
	constexpr float degree2Radius = 3.141592654f / 180.0f;
	constexpr float PIHalf = 3.141592654f / 2.0f;

	// イプシロン
	constexpr float EPSILON = 2.2204460492503131E-16f;
	
	/// <summary>
	/// 長さを求める(2d)
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Vector2Length(const KMyMath::Vector2& v);
	/// <summary>
	/// 長さを求める(3d)
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Vector3Length(const KMyMath::Vector3& v);

	float Vector3Dot(const KMyMath::Vector3& v1, const KMyMath::Vector3& v2);

	KMyMath::Vector3 Vector3Cross(const KMyMath::Vector3& v1, const KMyMath::Vector3& v2);

	/// <summary>
	/// Vector3正規化
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	KMyMath::Vector3 MakeNormalize(KMyMath::Vector3 v);

	// 単位行列
	KMyMath::Matrix4 MakeIdentity();

	/// <summary>
	/// 拡大行列
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeScaling(const KMyMath::Vector3& scale);

	/// <summary>
	/// 回転行列
	/// </summary>
	/// <param name="rotation"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeRotation(const KMyMath::Vector3& rotation);

	/// <summary>
	/// 平行行列
	/// </summary>
	/// <param name="trans"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeTranslation(const KMyMath::Vector3& trans);

	/// <summary>
	/// ワールド行列
	/// </summary>
	/// <param name="translation"></param>
	/// <param name="scaling"></param>
	/// <param name="rotation"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeWorld(KMyMath::Matrix4 translation, KMyMath::Matrix4 scaling, KMyMath::Matrix4 rotation);

	/// <summary>
	/// サインとコサイン
	/// </summary>
	/// <param name="sin_"></param>
	/// <param name="cos_"></param>
	/// <param name="angle"></param>
	void SinCos(float& sin_, float& cos_, float angle);

	/// <summary>
	/// 平行投影変換(左手)
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="top"></param>
	/// <param name="near_"></param>
	/// <param name="far_"></param>
	/// <param name="m"></param>
	KMyMath::Matrix4 MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_);

	/// <summary>
	/// 平行投影変換(右手)
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="top"></param>
	/// <param name="near_"></param>
	/// <param name="far_"></param>
	/// <param name="m"></param>
	KMyMath::Matrix4 MakeOrthogonalR(float left, float right, float bottom, float top, float near_, float far_);

	/// <summary>
	/// ビュー行列の作成
	/// </summary>
	/// <param name="eye"></param>
	/// <param name="target"></param>
	/// <param name="up"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeLockAt(KMyMath::Vector3& eye, KMyMath::Vector3& target, KMyMath::Vector3& up);

	/// <summary>
	/// 透視投影作成
	/// </summary>
	/// <param name="forAngleY"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="nearZ"></param>
	/// <param name="farZ"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakePerspective(float fogAngleY, float aspectRatio, float nearZ, float farZ);

	/// <summary>
	/// 逆行列
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	KMyMath::Matrix4 MakeInverse(KMyMath::Matrix4& mat);

	// Sin,Cos,Tan関数
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
	/// 値が等しいか
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	bool Approximately(float a, float b);

	KMyMath::Vector3 HermiteGetPoint(KMyMath::Vector3 p0, KMyMath::Vector3 p1, KMyMath::Vector3 v0, KMyMath::Vector3 v1, float t);

	/// <summary>
	/// ランダムな値を取得
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
    uint32_t GetRandI(const uint32_t& min, const uint32_t& max);
	float GetRandF(float min, float max);

	/// <summary>
	/// ベクトル変換
	/// </summary>
	/// <param name="v"></param>
	/// <param name="m"></param>
	/// <returns></returns>
	KMyMath::Vector3 TransforNormal(const KMyMath::Vector3& v, const KMyMath::Matrix4& m);

	/// <summary>
	/// 変換
	/// </summary>
	/// <param name="v"></param>
	/// <param name="m"></param>
	/// <returns></returns>
	KMyMath::Vector3 MatrixTransformWDivision(const KMyMath::Vector3& v, const KMyMath::Matrix4& m);
}

namespace KMyMath
{
	/// <summary>
	/// ベクトルと行列の掛け算
	/// </summary>
	/// <param name="vec"></param>
	/// <param name="mat"></param>
	/// <returns></returns>
	Vector3 Vec3Mat4Mul(Vector3& vec, Matrix4& mat);

	/// <summary>
	/// キャットロームスプライン曲線
	/// </summary>
	/// <param name="points"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatMullRomSprine(std::vector<Vector3>& points, float t);

	/// <summary>
	/// 分離軸に投影された軸成分から投影線分長を算出
	/// </summary>
	/// <param name="sep"></param>
	/// <param name="e1"></param>
	/// <param name="e2"></param>
	/// <param name="e3"></param>
	/// <returns></returns>
	float LenSegLineOfSeparateAxis(Vector3* sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);
}

namespace MyMathConvert
{
	/// <summary>
	/// Vector4からXMVECTORへ変換
	/// </summary>
	/// <param name="vector4"></param>
	/// <returns></returns>
	DirectX::XMVECTOR ChangeVector4toXMVECTOR(KMyMath::Vector4 vector4);

	/// <summary>
	/// XMVECTORからVector4へ変換
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	KMyMath::Vector4 ChangeXMVECTORtoVector4(DirectX::XMVECTOR vector);

	/// <summary>
	/// XMMATRIXからMatrix4へ変換
	/// </summary>
	/// <param name="matrix"></param>
	/// <returns></returns>
	KMyMath::Matrix4 ChangeXMMATRIXtoMatrix4(DirectX::XMMATRIX matrix);

	/// <summary>
	/// Matrix4からXMMATRIXへ変換
	/// </summary>
	/// <param name="matrix"></param>
	/// <returns></returns>
	DirectX::XMMATRIX ChangeMatrix4toXMMATRIX(KMyMath::Matrix4 matrix);

	/// <summary>
	/// XMFloat3からVector3へ変換
	/// </summary>
	/// <param name="float3"></param>
	/// <returns></returns>
	KMyMath::Vector3 ChangeXMFloat3toVector3(DirectX::XMFLOAT3 float3);

	/// <summary>
	/// Vector3からXMFloat3へ変換
	/// </summary>
	/// <param name="vector3"></param>
	/// <returns></returns>
	DirectX::XMFLOAT3 ChangeVector3toXMfloat3(KMyMath::Vector3 vector3);
}