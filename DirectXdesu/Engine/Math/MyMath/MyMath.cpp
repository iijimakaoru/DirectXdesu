#include "MyMath.h"
#include <cmath>
#include <cassert>
#include <random>

namespace MyMathUtility
{
	float Vector2Length(const KMyMath::Vector2& v)
	{
		return static_cast<float>(std::sqrt(v.x * v.x + v.y * v.y));
	}

	float Vector3Length(const KMyMath::Vector3& v)
	{
		return static_cast<float>(std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
	}

	KMyMath::Vector3 MakeNormalize(KMyMath::Vector3 v)
	{
		float len = Vector3Length(v);
		if (len != 0)
		{
			return v /= len;
		}
		return v;
	}

	KMyMath::Matrix4 MakeIdentity()
	{
		KMyMath::Matrix4 matIdentity;
		matIdentity.m[0][0] = 1.0f;
		matIdentity.m[1][1] = 1.0f;
		matIdentity.m[2][2] = 1.0f;
		matIdentity.m[3][3] = 1.0f;

		return matIdentity;
	}

	KMyMath::Matrix4 MakeScaling(const KMyMath::Vector3& scale)
	{
		KMyMath::Matrix4 matScale;

		matScale.m[0][0] = scale.x;
		matScale.m[1][1] = scale.y;
		matScale.m[2][2] = scale.z;
		matScale.m[3][3] = 1.0f;

		return matScale;
	}

	KMyMath::Matrix4 MakeRotation(const KMyMath::Vector3& rotation)
	{
		KMyMath::Matrix4 matRotationX;
		matRotationX = MakeIdentity();
		matRotationX.m[1][1] = cos(rotation.x);
		matRotationX.m[1][2] = sin(rotation.x);
		matRotationX.m[2][1] = -sin(rotation.x);
		matRotationX.m[2][2] = cos(rotation.x);

		KMyMath::Matrix4 matRotationY;
		matRotationY = MakeIdentity();
		matRotationY.m[0][0] = cos(rotation.y);
		matRotationY.m[0][2] = -sin(rotation.y);
		matRotationY.m[2][0] = sin(rotation.y);
		matRotationY.m[2][2] = cos(rotation.y);

		KMyMath::Matrix4 matRotationZ;
		matRotationZ = MakeIdentity();
		matRotationZ.m[0][0] = cos(rotation.z);
		matRotationZ.m[0][1] = sin(rotation.z);
		matRotationZ.m[1][0] = -sin(rotation.z);
		matRotationZ.m[1][1] = cos(rotation.z);

		KMyMath::Matrix4 matRotation;
		matRotation = MakeIdentity();
		matRotation *= matRotationZ;
		matRotation *= matRotationX;
		matRotation *= matRotationY;

		return matRotation;
	}

	KMyMath::Matrix4 MakeTranslation(const KMyMath::Vector3& trans)
	{
		KMyMath::Matrix4 matTranslation;
		matTranslation = MakeIdentity();
		matTranslation.m[3][0] = trans.x;
		matTranslation.m[3][1] = trans.y;
		matTranslation.m[3][2] = trans.z;

		return matTranslation;
	}

	KMyMath::Matrix4 MakeWorld(KMyMath::Matrix4 translation, KMyMath::Matrix4 scaling, KMyMath::Matrix4 rotation)
	{
		KMyMath::Matrix4 result;

		result = scaling * rotation * translation;

		return result;
	}

	void SinCos(float& sin_, float& cos_, float angle)
	{
		sin_ = Sin(angle);
		cos_ = Cos(angle);
	}

	KMyMath::Matrix4 MatMulVector(KMyMath::Matrix4 m, KMyMath::Vector3 v)
	{
		return KMyMath::Matrix4();
	}

	KMyMath::Matrix4 MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_)
	{
		float width = 1.0f / (right - left);
		float height = 1.0f / (top - bottom);
		float range = 1.0f / (far_ - near_);

		KMyMath::Matrix4 matrix;
		matrix.m[0][0] = width * 2;
		matrix.m[1][1] = height * 2;
		matrix.m[2][2] = range;
		matrix.m[3][0] = -(left + right) * width;
		matrix.m[3][1] = -(top + bottom) * height;
		matrix.m[3][2] = range * -near_;
		matrix.m[3][3] = 1.0f;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][3] = 0.0f;

		return matrix;
	}

	KMyMath::Matrix4 MakeOrthogonalR(float left, float right, float bottom, float top, float near_, float far_)
	{
		float width = 1.0f / (right - left);
		float height = 1.0f / (top - bottom);
		float range = 1.0f / (far_ - near_);

		KMyMath::Matrix4 matrix;
		matrix.m[0][0] = width * 2;
		matrix.m[1][1] = height * 2;
		matrix.m[2][2] = range;
		matrix.m[3][0] = -(left + right) * width;
		matrix.m[3][1] = -(top + bottom) * height;
		matrix.m[3][2] = range * -near_;
		matrix.m[3][3] = 1.0f;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][3] = 0.0f;

		return matrix;
	}

	KMyMath::Matrix4 MakeLockAt(KMyMath::Vector3& eye, KMyMath::Vector3& target, KMyMath::Vector3& up)
	{
		KMyMath::Vector3 zVec = target - eye;
		zVec.Normalize();

		KMyMath::Vector3 xVec = up.Cross(zVec);
		xVec.Normalize();

		KMyMath::Vector3 yVec = zVec.Cross(xVec);
		yVec.Normalize();

		KMyMath::Matrix4 matrix;
		matrix.m[0][0] = xVec.x;
		matrix.m[0][1] = xVec.y;
		matrix.m[0][2] = xVec.z;

		matrix.m[1][0] = yVec.x;
		matrix.m[1][1] = yVec.y;
		matrix.m[1][2] = yVec.z;

		matrix.m[2][0] = zVec.x;
		matrix.m[2][1] = zVec.y;
		matrix.m[2][2] = zVec.z;

		matrix.m[3][0] = -eye.x;
		matrix.m[3][1] = -eye.y;
		matrix.m[3][2] = -eye.z;

		return matrix;
	}

	KMyMath::Matrix4 MakePerspective(float fogAngleY, float aspectRatio, float nearZ, float farZ)
	{
		// アスペクト比を作成
		KMyMath::Matrix4 matrix;
		float sinFov = 0.0f;
		float cosFov = 0.0f;
		SinCos(sinFov, cosFov, 0.5f * fogAngleY);

		float range = farZ / (farZ - nearZ);
		float height = cosFov / sinFov;

		matrix.m[0][0] = height / aspectRatio;

		matrix.m[1][1] = cosFov / sinFov;

		matrix.m[2][2] = range;
		matrix.m[2][3] = 1.0f;

		matrix.m[3][2] = -range * nearZ;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] =
			matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][3] = 0.0f;

		return matrix;
	}

	KMyMath::Matrix4 MakeInverse(KMyMath::Matrix4& mat)
	{
		//掃き出し法を行う行列
		float sweep[4][8]{};
		//定数倍用
		float constTimes = 0.0f;
		//許容する誤差
		float MAX_ERR = 1e-10f;
		//戻り値用
		KMyMath::Matrix4 retMat;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				//weepの左側に逆行列を求める行列をセット
				sweep[i][j] = mat.m[i][j];

				//sweepの右側に単位行列をセット
				sweep[i][4 + j] = MakeIdentity().m[i][j];
			}
		}

		//全ての列の対角成分に対する繰り返し
		for (int i = 0; i < 4; i++)
		{
			//最大の絶対値を注目対角成分の絶対値と仮定
			float max = std::fabs(sweep[i][i]);
			int maxIndex = i;

			//i列目が最大の絶対値となる行を探す
			for (int j = i + 1; j < 4; j++)
			{
				if (std::fabs(sweep[j][i]) > max)
				{
					max = std::fabs(sweep[j][i]);
					maxIndex = j;
				}
			}

			if (fabs(sweep[maxIndex][i]) <= MAX_ERR)
			{
				//逆行列は求められない
				return MakeIdentity();
			}

			//操作(1):i行目とmaxIndex行目を入れ替える
			if (i != maxIndex)
			{
				for (int j = 0; j < 8; j++)
				{
					float tmp = sweep[maxIndex][j];
					sweep[maxIndex][j] = sweep[i][j];
					sweep[i][j] = tmp;
				}
			}

			//sweep[i][i]に掛けると1になる値を求める
			constTimes = 1 / sweep[i][i];

			//操作(2):p行目をa倍する
			for (int j = 0; j < 8; j++)
			{
				//これによりsweep[i][i]が1になる
				sweep[i][j] *= constTimes;
			}

			//操作(3)によりi行目以外の行のi列目を0にする
			for (int j = 0; j < 4; j++)
			{
				if (j == i)
				{
					//i行目はそのまま
					continue;
				}

				//i行目に掛ける値を求める
				constTimes = -sweep[j][i];

				for (int k = 0; k < 8; k++)
				{
					//j行目にi行目をa倍した行を足す
					//これによりsweep[j][i]が0になる
					sweep[j][k] += sweep[i][k] * constTimes;
				}
			}
		}

		//sweepの右半分がmatの逆行列
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				retMat.m[i][j] = sweep[i][4 + j];
			}
		}

		return retMat;
	}

	float Sin(float sin)
	{
		return std::sinf(sin);
	}

	float Cos(float cos)
	{
		return std::cosf(cos);
	}

	float Tan(float tan)
	{
		return std::tanf(tan);
	}

	float Asin(float sin)
	{
		return std::asinf(sin);
	}

	float Acos(float cos)
	{
		return std::acosf(cos);
	}

	float Atan(float tan)
	{
		return std::atanf(tan);
	}

	float Atan2(float y, float x)
	{
		return std::atan2f(y, x);
	}

	void Complement(float& x1, float x2, float flame)
	{
		//距離を出す
		float distanceX = x2 - x1;
		//距離をflameで割った値
		float dividedDistanceX = distanceX / flame;

		//距離をflameで割った値を足す
		x1 += dividedDistanceX;
	}

	float Clamp(float Value, const float low, const float high)
	{
		if (high < Value)
		{
			Value = high;
		}

		if (Value < low)
		{
			Value = low;
		}

		return Value;
	}

	float Clamp0To1(float val)
	{
		if (val < 0.0f)
		{
			return 0.0f;
		}
		if (val > 1.0f)
		{
			return 1.0f;
		}

		return val;
	}

	bool Approximately(float a, float b)
	{
		float tmp = 1e-06f * std::max(fabs(a), fabs(b));

		float tmp2 = EPSILON * 8.0f;

		if (fabs(b - a) < std::max(tmp, tmp2))
		{
			return true;
		}

		return false;
	}

	KMyMath::Vector3 HermiteGetPoint(KMyMath::Vector3 p0, KMyMath::Vector3 p1, KMyMath::Vector3 v0, KMyMath::Vector3 v1, float t)
	{
		KMyMath::Vector3 c0 = 2.0f * p0 + -2.0f * p1 + v0 + v1;
		KMyMath::Vector3 c1 = -3.0f * p0 + 3.0f * p1 + -2.0f * v0 - v1;
		KMyMath::Vector3 c2 = v0;
		KMyMath::Vector3 c3 = p0;

		float t2 = t * t;
		float t3 = t2 * t;
		return c0 * t3 + c1 * t2 + c2 * t + c3;
	}

	float GetRand(float min, float max)
	{
		// 乱数生成器
		static std::random_device rd;
		static std::default_random_engine eng(rd());

		// 分布生成器
		std::uniform_real_distribution<float> get_rand_uni_real(min, max);

		// 乱数を生成
		return get_rand_uni_real(eng);
	}
}

namespace KMyMath
{
	Vector3 Vec3Mat4Mul(Vector3& vec, Matrix4& mat)
	{
		Vector3 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];
		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];
		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];

		return retVec;
	}

	Vector3 CatMullRomSprine(std::vector<Vector3>& points, float t)
	{
		float length = static_cast<float>(points.size());
		float progress = (length - 1) * t;
		float index = std::floor(progress);
		float weight = progress - index;

		if (MyMathUtility::Approximately(weight, 0.0f) && index >= length - 1)
		{
			index = length - 2;
			weight = 1;
		}

		Vector3 p0 = points[static_cast<size_t>(index)];
		Vector3 p1 = points[static_cast<size_t>(index + 1.0f)];
		Vector3 p2;
		Vector3 p3;

		if (index > 0.0f)
		{
			p2 = 0.5f * (points[static_cast<size_t>(index + 1.0f)] - points[static_cast<size_t>(index - 1.0f)]);
		}
		else
		{
			p2 = points[static_cast<size_t>(index + 1.0f)] - points[static_cast<size_t>(index)];
		}

		if (index < length - 2.0f)
		{
			p3 = 0.5f * (points[static_cast<size_t>(index + 2.0f)] - points[static_cast<size_t>(index)]);
		}
		else
		{
			p3 = points[static_cast<size_t>(index + 1.0f)] - points[static_cast<size_t>(index)];
		}

		return MyMathUtility::HermiteGetPoint(p0, p1, p2, p3, weight);
	}

	float LenSegLineOfSeparateAxis(Vector3* sep, Vector3* e1, Vector3* e2, Vector3* e3)
	{
		// 3つの内積の絶対値の和で投影線分長を計算
		float r1 = fabs(sep->Dot(*e1));
		float r2 = fabs(sep->Dot(*e2));
		float r3 = e3 ? (fabs(sep->Dot(*e3))) : 0;
		return r1 + r2 + r3;
	}
}

namespace MyMathConvert
{
	DirectX::XMVECTOR ChangeVector4toXMVECTOR(KMyMath::Vector4 vector4)
	{
		DirectX::XMVECTOR result;
		result.m128_f32[0] = vector4.x;
		result.m128_f32[1] = vector4.y;
		result.m128_f32[2] = vector4.z;
		result.m128_f32[3] = vector4.w;

		return result;
	}

	KMyMath::Vector4 ChangeXMVECTORtoVector4(DirectX::XMVECTOR vector)
	{
		KMyMath::Vector4 result;
		result.x = vector.m128_f32[0];
		result.y = vector.m128_f32[1];
		result.z = vector.m128_f32[2];
		result.w = vector.m128_f32[3];

		return result;
	}

	KMyMath::Matrix4 ChangeXMMATRIXtoMatrix4(DirectX::XMMATRIX matrix)
	{
		KMyMath::Matrix4 result;

		for (size_t i = 0; i < 4; i++)
		{
			result.m[i][0] = DirectX::XMVectorGetX(matrix.r[i]);
			result.m[i][1] = DirectX::XMVectorGetY(matrix.r[i]);
			result.m[i][2] = DirectX::XMVectorGetZ(matrix.r[i]);
			result.m[i][3] = DirectX::XMVectorGetW(matrix.r[i]);
		}

		return result;
	}

	DirectX::XMMATRIX ChangeMatrix4toXMVECTOR(KMyMath::Matrix4 matrix)
	{
		DirectX::XMMATRIX result;

		result = DirectX::XMMatrixSet(
			matrix.m[0][0], matrix.m[0][1], matrix.m[0][2], matrix.m[0][3],
			matrix.m[1][0], matrix.m[1][1], matrix.m[1][2], matrix.m[1][3],
			matrix.m[2][0], matrix.m[2][1], matrix.m[2][2], matrix.m[2][3],
			matrix.m[3][0], matrix.m[3][1], matrix.m[3][2], matrix.m[3][3]);

		return result;
	}

	KMyMath::Vector3 ChangeXMFloat3toVector3(DirectX::XMFLOAT3 float3)
	{
		KMyMath::Vector3 result;

		result.x = float3.x;
		result.y = float3.y;
		result.z = float3.z;

		return result;
	}

	DirectX::XMFLOAT3 ChangeVector3toXMfloat3(KMyMath::Vector3 vector3)
	{
		DirectX::XMFLOAT3 result;

		result.x = vector3.x;
		result.y = vector3.y;
		result.z = vector3.z;

		return result;
	}
}