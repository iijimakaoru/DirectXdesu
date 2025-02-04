#pragma once
#include<cstdint>

namespace MCBM
{
	class MVector4
	{
	public:
		MVector4();
		MVector4(float x, float y, float z, float w);
		MVector4(int32_t x, int32_t y, int32_t z, int32_t w);
		MVector4(size_t x, size_t y, size_t z, size_t w);
		MVector4(float values[4]);

	public:

		void Set(float value);

		void SetX(float x);
		void SetX(size_t x);
		void SetX(int32_t x);

		void SetY(float y);
		void SetY(size_t y);
		void SetY(int32_t y);

		void SetZ(float z);
		void SetZ(size_t z);
		void SetZ(int32_t z);

		void SetW(float w);
		void SetW(size_t w);
		void SetW(int32_t w);

		void SetValue(float x, float y, float z, float w);
		void SetValue(size_t x, size_t y, size_t z, size_t w);
		void SetValue(int32_t x, int32_t y, int32_t z, size_t w);

		void SetValues(float values[4]);
		void SetValues(size_t values[4]);
		void SetValues(int32_t values[4]);

		float GetX() const;
		float GetY() const;
		float GetZ() const;
		float GetW() const;

		const float* GetValues() const;

		float Length()const;
		MVector4 Normalization()const;
		MVector4& Normalization();
		float Dot(const MVector4& v)const;
		MVector4 Cross(const MVector4& v1, const MVector4& v2)const;

		float Vector3Length()const;
		MVector4 Vector3Normalization()const;
		MVector4& Vector3Normalization();
		float Vector3Dot(const MVector4& v)const;
		MVector4 Vector3Cross(const MVector4& v)const;

	public:

		MVector4 operator+() const;
		MVector4 operator-() const;

		MVector4& operator+=(const MVector4& v);
		MVector4& operator-=(const MVector4& v);
		MVector4& operator*=(float s);
		MVector4& operator/=(float s);

		friend const MVector4 operator+(const MVector4& v1, const MVector4& v2);
		friend const MVector4 operator-(const MVector4& v1, const MVector4& v2);
		friend const MVector4 operator*(const MVector4& v, float s);
		friend const MVector4 operator*(float s, const MVector4& v);
		friend const MVector4 operator/(const MVector4& v, float s);

	private:

		struct VEC
		{
			float x;
			float y;
			float z;
			float w;
		};

		union VALUE
		{
			float elements[4];

			VEC vec;
		};

	private:

		VALUE m_value;
	};
}