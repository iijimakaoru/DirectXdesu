#pragma once
namespace MCBM
{

	class MVector3
	{
	public:
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		MVector3(const MVector3& start, const MVector3& end);
		MVector3();
		MVector3(float x, float y, float z);

		MVector3 Vector3Substruct(const MVector3& start, const MVector3& end);
		float V3Len() const;
		void V3Norm();
		MVector3 GetV3Norm();
		float GetV3Dot(const MVector3& vector);
		float GetInnerProduct(MVector3 vector);
		MVector3 GetV3Cross(const MVector3& vector);
		MVector3 GetV3Cross(const MVector3& aVector, const MVector3& bVector);


		MVector3 GetFrontVec(MVector3 RightVec, MVector3 UpVec = { 0,1,0 });
		MVector3 GetUpVec(MVector3 RightVec, MVector3 frontVec = { 0,0,1 });
		MVector3 GetRightVec(MVector3 frontVec, MVector3 UpVec = { 0,1,0 });

		MVector3 GetV3Normal(MVector3 v0, MVector3 v1, MVector3 v2);

		MVector3 Qlamp(const MVector3& vec, const MVector3& min, const MVector3& max);

		MVector3& operator+=(const MVector3& Vec);
		MVector3& operator-=(const MVector3& Vec);
		MVector3& operator*=(float k);

		MVector3 Clamp(const MVector3& vec1, const MVector3& min, const MVector3& max, bool* isClamped = nullptr);

		MVector3 adjustVectorLength(const MVector3& base, const MVector3& target);

	};
	MVector3 operator+(const MVector3& vecA, const MVector3& vecB);
	bool operator!=(const MVector3& vecA, const MVector3& vecB);
	MVector3 operator-(const MVector3& vecA, const MVector3& vecB);
	MVector3 operator*(const MVector3& vecA, float k);
	MVector3 operator*(float k, const MVector3& vecA);
	MVector3 operator/(const MVector3& vecA, float k);
	MVector3 operator-(const MVector3& vecA);


}

