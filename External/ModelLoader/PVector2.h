#ifndef	__MATH_P_VECTOR2_H__
#define	__MATH_P_VECTOR2_H__

#include<P_Defined.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cstdint>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

class PVector2
{
public:

	PVector2();
	PVector2(float x,float y);
	PVector2(size_t x,size_t y);
	PVector2(int32_t x,int32_t y);

public:

	void SetX(float x);
	void SetX(size_t x);
	void SetX(int32_t x);

	void SetY(float y);
	void SetY(size_t y);
	void SetY(int32_t y);

	void SetValue(float x,float y);
	void SetValue(size_t x,size_t y);
	void SetValue(int32_t x,int32_t y);

	void SetValues(float values[ 2 ]);
	void SetValues(size_t values[ 2 ]);
	void SetValues(int32_t values[ 2 ]);

	float GetX() const;
	float GetY() const;
	const float* GetValues() const;

	float Length()const;
	PVector2& Normalization();
	float Dot(const PVector2& v)const;
	float Cross(const PVector2& v)const;

public:

	PVector2 operator+()const;
	PVector2 operator-()const;

	PVector2& operator+=(const PVector2& v);
	PVector2& operator-=(const PVector2& v);
	PVector2& operator*=(float s);
	PVector2& operator/=(float s);
	PVector2& operator/=(const PVector2& v);

	bool operator==(const PVector2& v);

	friend const PVector2 operator+(const PVector2& v1,const PVector2& v2);
	friend const PVector2 operator-(const PVector2& v1,const PVector2& v2);
	friend const PVector2 operator*(const PVector2& v,float s);
	friend const PVector2 operator*(float s,const PVector2& v);
	friend const PVector2 operator/(const PVector2& v,float s);

private:

	struct VEC
	{
		float x;
		float y;
	};

	union VALUE
	{
		float elements[ 2 ];

		VEC vec;
	};

private:

	VALUE m_value;

};

PHONON_LOADER_NAMESPACE_END

#endif	// __MATH_P_VECTOR2_H__