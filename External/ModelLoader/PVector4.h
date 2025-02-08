#ifndef	__MATH_P_VECTOR4_H__
#define	__MATH_P_VECTOR4_H__

#include<P_Defined.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cstdint>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

class PVector4
{
public:
	PVector4();
	PVector4(float x,float y,float z,float w);
	PVector4(int32_t x,int32_t y,int32_t z,int32_t w);
	PVector4(size_t x,size_t y,size_t z,size_t w);
	PVector4(float values[ 4 ]);

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

	void SetValue(float x,float y,float z,float w);
	void SetValue(size_t x,size_t y,size_t z,size_t w);
	void SetValue(int32_t x,int32_t y,int32_t z,size_t w);

	void SetValues(float values[ 4 ]);
	void SetValues(size_t values[ 4 ]);
	void SetValues(int32_t values[ 4 ]);

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;

	const float* GetValues() const;

	float Length()const;
	PVector4 Normalization()const;
	PVector4& Normalization();
	float Dot(const PVector4& v)const;
	PVector4 Cross(const PVector4& v1,const PVector4& v2)const;

	float Vector3Length()const;
	PVector4 Vector3Normalization()const;
	PVector4& Vector3Normalization();
	float Vector3Dot(const PVector4& v)const;
	PVector4 Vector3Cross(const PVector4& v)const;

public:

	PVector4 operator+() const;
	PVector4 operator-() const;

	PVector4& operator+=(const PVector4& v);
	PVector4& operator-=(const PVector4& v);
	PVector4& operator*=(float s);
	PVector4& operator/=(float s);

	friend const PVector4 operator+(const PVector4& v1,const PVector4& v2);
	friend const PVector4 operator-(const PVector4& v1,const PVector4& v2);
	friend const PVector4 operator*(const PVector4& v,float s);
	friend const PVector4 operator*(float s,const PVector4& v);
	friend const PVector4 operator/(const PVector4& v,float s);

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
		float elements[ 4 ];

		VEC vec;
	};

private:

	VALUE m_value;
};

PHONON_LOADER_NAMESPACE_END

#endif	// __MATH_P_VECTOR4_H__

