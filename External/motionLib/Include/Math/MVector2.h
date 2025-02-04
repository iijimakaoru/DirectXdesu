#include<cstdint>

namespace MCBM
{
	class MVector2
	{
	public:

		MVector2();
		MVector2(float x, float y);
		MVector2(size_t x, size_t y);
		MVector2(int32_t x, int32_t y);

	public:

		void SetX(float x);
		void SetX(size_t x);
		void SetX(int32_t x);

		void SetY(float y);
		void SetY(size_t y);
		void SetY(int32_t y);

		void SetValue(float x, float y);
		void SetValue(size_t x, size_t y);
		void SetValue(int32_t x, int32_t y);

		void SetValues(float values[2]);
		void SetValues(size_t values[2]);
		void SetValues(int32_t values[2]);

		float GetX() const;
		float GetY() const;
		const float* GetValues() const;

		float Length()const;
		MVector2& Normalization();
		float Dot(const MVector2& v)const;
		float Cross(const MVector2& v)const;

	public:

		MVector2 operator+()const;
		MVector2 operator-()const;

		MVector2& operator+=(const MVector2& v);
		MVector2& operator-=(const MVector2& v);
		MVector2& operator*=(float s);
		MVector2& operator/=(float s);
		MVector2& operator/=(const MVector2& v);

		friend const MVector2 operator+(const MVector2& v1, const MVector2& v2);
		friend const MVector2 operator-(const MVector2& v1, const MVector2& v2);
		friend const MVector2 operator*(const MVector2& v, float s);
		friend const MVector2 operator*(float s, const MVector2& v);
		friend const MVector2 operator/(const MVector2& v, float s);

	private:

		struct VEC
		{
			float x;
			float y;
		};

		union VALUE
		{
			float elements[2];

			VEC vec;
		};

	private:

		VALUE m_value;

	};
}