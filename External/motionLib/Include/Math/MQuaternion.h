#pragma once
#include <Math/MVector3.h>
#include <stdint.h>

namespace MCBM
{
	class MQuaternion
	{
	public:
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
		float w = 1.f;
		MQuaternion(const MVector3& vec, float angle);
		MQuaternion();
		MQuaternion(float x, float y, float z, float w);

		//与えられたベクトル周りの回転を表すクォータニオンを生成
		void SetRota(MVector3 vec, float angle);
		//共役
		MQuaternion GetConjugated(MQuaternion q);
		//逆数（数学用語より
		MQuaternion GetReciprocal(MQuaternion q);
		//長さ
		double GetNorm();
		//q*pの直積
		MQuaternion GetDirectProduct(const MQuaternion& q, const MQuaternion& p);
		//内積
		float Dot(const MQuaternion& a, const MQuaternion& b);
		//二つのクォータニオンの角度差
		float GetAngle(const MQuaternion& a, const MQuaternion& b, float& dot, bool& nan);
		float GetAngle(const MQuaternion& a, const MQuaternion& b);
		//球面線形補間
		MQuaternion Slerp(MQuaternion start, const MQuaternion& end, int32_t time, int32_t maxTime);
		//球面線形補間
		MQuaternion Slerp(MQuaternion start, MQuaternion end, float time);
		//与えられたベクトル軸周りの回転をPositionVecに与える
		MVector3 SetRotationVector(const MVector3& rotationAxisVec, MVector3 PositionVec, float angle);
		//与えられたクォータニオンの回転をPositionVecに与える
		MVector3 SetRotationVector(const MQuaternion& rotationQuaternion, MVector3 PositionVec);


		MVector3 SetRotationVector(const MQuaternion& rotationQuaternion, const MQuaternion& PositionVec);

		MQuaternion DirToDir(MVector3 u, MVector3 v);
		//正規化する
		void Normalize();
		//正規化した値を返す
		static MQuaternion Normalize(MQuaternion q);
		

		void SinCos(float* returnSin, float* returnCos, float theta);
		//オイラー角からクォータニオンを生成
		MQuaternion SetToRorateObjectToInternal(const MVector3& eulerAngle);
		//クォータニオンの回転軸を算出
		MVector3 GetRotationAxis(const MQuaternion& q);
		//クォータニオンの回転軸を算出(AxisVecを書き換える)
		void GetRotationAxis(const MQuaternion& q, MVector3& AxisVec);
		//単位クォータニオン
		MQuaternion Identity();

		//クォータニオンの角度
		float GetAngle(const MQuaternion& q);

		float SafeAcos(float a);

		bool operator== (const MQuaternion& q);
		MQuaternion operator-();
		MQuaternion operator*(float k);
		MQuaternion operator+(MQuaternion q);
	};
	MQuaternion operator*(float k, MQuaternion q);
	MQuaternion SetRota(const MVector3& vec, float angle);
}

