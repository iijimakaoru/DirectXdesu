#ifndef	__MATH_P_VECTOR3_H__
#define	__MATH_P_VECTOR3_H__

#include<P_Defined.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cstdint>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

class PVector3
{
public:

	PVector3();
	PVector3(float x,float y,float z);
	PVector3(int32_t x,int32_t y,int32_t z);
	PVector3(size_t x,size_t y,size_t z);
	PVector3(float values[ 3 ]);

public:

	void SetX(float x);
	void SetX(size_t x);
	void SetX(int32_t x);

	void SetY(float y);
	void SetY(size_t y);
	void SetY(int32_t y);

	void SetZ(float z);
	void SetZ(size_t z);
	void SetZ(int32_t z);

	void SetValue(float x,float y,float z);
	void SetValue(size_t x,size_t y, size_t z);
	void SetValue(int32_t x,int32_t y,int32_t z);

	void SetValues(float values[ 3 ]);
	void SetValues(size_t values[ 3 ]);
	void SetValues(int32_t values[ 3 ]);

	float GetX() const;
	float GetY() const;
	float GetZ() const;

	const float* GetValues() const;

	float Length()const;
	PVector3 Normalization()const;
	PVector3& Normalization();
	float Dot(const PVector3& v)const;
	PVector3 Cross(const PVector3& v)const;
	PVector3 Negate() const;

public:

	PVector3 operator+() const;
	PVector3 operator-() const;

	PVector3& operator+=(const PVector3& v);
	PVector3& operator-=(const PVector3& v);
	PVector3& operator*=(const PVector3& v);
	PVector3& operator-=(float s);
	PVector3& operator*=(float s);
	PVector3& operator/=(float s);
	bool operator==(const PVector3& v)const;
	bool operator!=(const PVector3& v) const;

	friend const PVector3 operator+(const PVector3& v1,const PVector3& v2);
	friend const PVector3 operator-(const PVector3& v1,const PVector3& v2);
	friend const PVector3 operator-(const PVector3& v,float s);
	friend const PVector3 operator*(const PVector3& v,float s);
	friend const PVector3 operator*(const PVector3& v,const PVector3& v2);
	friend const PVector3 operator*(float s,const PVector3& v);
	friend const PVector3 operator/(const PVector3& v,float s);

private:

	struct VEC
	{
		float x;
		float y;
		float z;
	};

	union VALUE
	{
		float elements[ 3 ];

		VEC vec;
	};

private:

	VALUE m_value;

};

PHONON_LOADER_NAMESPACE_END

#endif	// __MATH_P_VECTOR3_H__
