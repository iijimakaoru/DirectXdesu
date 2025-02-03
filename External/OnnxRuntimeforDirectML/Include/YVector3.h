#pragma once
namespace MCBO
{

	class YVector3
	{
	public:
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		YVector3(const YVector3& start, const YVector3& end);
		YVector3();
		YVector3(float x, float y, float z);

		YVector3 Vector3Substruct(const YVector3& start, const YVector3& end);
		float V3Len() const;
		void V3Norm();
		YVector3 GetV3Norm();
		float GetV3Dot(const YVector3& vector);
		float GetInnerProduct(YVector3 vector);
		YVector3 GetV3Cross(const YVector3& vector);
		YVector3 GetV3Cross(const YVector3& aVector, const YVector3& bVector);


		YVector3 GetFrontVec(YVector3 RightVec, YVector3 UpVec = { 0,1,0 });
		YVector3 GetUpVec(YVector3 RightVec, YVector3 frontVec = { 0,0,1 });
		YVector3 GetRightVec(YVector3 frontVec, YVector3 UpVec = { 0,1,0 });

		YVector3 GetV3Normal(YVector3 v0, YVector3 v1, YVector3 v2);

		YVector3 Qlamp(const YVector3& vec, const YVector3& min, const YVector3& max);

		YVector3& operator+=(const YVector3& Vec);
		YVector3& operator-=(const YVector3& Vec);
		YVector3& operator*=(float k);

		YVector3 Clamp(const YVector3& vec1, const YVector3& min, const YVector3& max, bool* isClamped = nullptr);

	};
	YVector3 operator+(const YVector3& vecA, const YVector3& vecB);
	bool operator!=(const YVector3& vecA, const YVector3& vecB);
	YVector3 operator-(const YVector3& vecA, const YVector3& vecB);
	YVector3 operator*(const YVector3& vecA, float k);
	YVector3 operator*(float k, const YVector3& vecA);
	YVector3 operator/(const YVector3& vecA, float k);
	YVector3 operator-(const YVector3& vecA);


}

