#include <CaptureVector3.h>

#include<cmath>
#include<algorithm>

CaptureVector3::CaptureVector3()
{
	m_value.vec.x = 0.0f;
	m_value.vec.y = 0.0f;
	m_value.vec.z = 0.0f;
}

CaptureVector3::CaptureVector3(float x,float y,float z)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
}

CaptureVector3::CaptureVector3(int32_t x,int32_t y,int32_t z)
{
	m_value.vec.x = static_cast< float > ( x );
	m_value.vec.y = static_cast< float > ( y );
	m_value.vec.z = static_cast< float > ( z );
}

CaptureVector3::CaptureVector3(size_t x,size_t y,size_t z)
{
	m_value.vec.x = static_cast< float > ( x );
	m_value.vec.y = static_cast< float > ( y );
	m_value.vec.z = static_cast< float > ( z );
}

CaptureVector3::CaptureVector3(float values[ 3 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
	m_value.vec.z = values[ 2 ];
}

void CaptureVector3::Set(size_t index,float val)
{
	m_value.elements[index] = val;
}

void CaptureVector3::SetX(float x)
{
	m_value.vec.x = x;
}

void CaptureVector3::SetX(size_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void CaptureVector3::SetX(int32_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void CaptureVector3::SetY(float y)
{
	m_value.vec.y = y;
}

void CaptureVector3::SetY(size_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void CaptureVector3::SetY(int32_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void CaptureVector3::SetZ(float z)
{
	m_value.vec.z = z;
}

void CaptureVector3::SetZ(size_t z)
{
	m_value.vec.z = static_cast< float >( z );
}

void CaptureVector3::SetZ(int32_t z)
{
	m_value.vec.z = static_cast< float >( z );
}

void CaptureVector3::SetValue(float x,float y,float z)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
}

void CaptureVector3::SetValue(size_t x,size_t y,size_t z)
{
	m_value.vec.x = static_cast< float > ( x );
	m_value.vec.y = static_cast< float > ( y );
	m_value.vec.z = static_cast< float > ( z );
}

void CaptureVector3::SetValue(int32_t x,int32_t y,int32_t z)
{
	m_value.vec.x = static_cast< float > ( x );
	m_value.vec.y = static_cast< float > ( y );
	m_value.vec.z = static_cast< float > ( z );
}

void CaptureVector3::SetValues(float values[ 3 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
	m_value.vec.z = values[ 2 ];
}

void CaptureVector3::SetValues(size_t values[ 3 ])
{
	m_value.vec.x = static_cast< float > ( values[ 0 ] );
	m_value.vec.y = static_cast< float > ( values[ 1 ] );
	m_value.vec.z = static_cast< float > ( values[ 2 ] );
}

void CaptureVector3::SetValues(int32_t values[ 3 ])
{
	m_value.vec.x = static_cast< float > ( values[ 0 ] );
	m_value.vec.y = static_cast< float > ( values[ 1 ] );
	m_value.vec.z = static_cast< float > ( values[ 2 ] );
}

float CaptureVector3::GetX() const
{
	return m_value.vec.x;
}

float CaptureVector3::GetY() const
{
	return m_value.vec.y;
}

float CaptureVector3::GetZ() const
{
	return m_value.vec.z;
}

const float* CaptureVector3::GetValues() const
{
	return m_value.elements;
}


float CaptureVector3::Length() const
{
	return std::sqrt(m_value.vec.x * m_value.vec.x + m_value.vec.y * m_value.vec.y + m_value.vec.z * m_value.vec.z);
}

CaptureVector3& CaptureVector3::Normalization()
{
	float len = Length();

	if ( len != 0 )
	{
		return *this /= len;
	}

	return *this;
}

CaptureVector3 CaptureVector3::Normalization() const
{
	CaptureVector3 tmp(*this);

	float len = Length();

	if ( len != 0 )
	{
		return tmp /= len;
	}

	return tmp;
}

float CaptureVector3::Dot(const CaptureVector3& v) const
{
	return m_value.vec.x * v.m_value.vec.x + m_value.vec.y * v.m_value.vec.y + m_value.vec.z * v.m_value.vec.z;
}

CaptureVector3 CaptureVector3::Cross(const CaptureVector3& v) const
{
	CaptureVector3 w;

	w.m_value.vec.x = m_value.vec.y * v.m_value.vec.z - m_value.vec.z * v.m_value.vec.y;
	w.m_value.vec.y = m_value.vec.z * v.m_value.vec.x - m_value.vec.x * v.m_value.vec.z;
	w.m_value.vec.z = m_value.vec.x * v.m_value.vec.y - m_value.vec.y * v.m_value.vec.x;

	return w;
}

CaptureVector3 CaptureVector3::Negate() const
{
	CaptureVector3 retVec;
	retVec.m_value.vec.x = -m_value.vec.x;
	retVec.m_value.vec.y = -m_value.vec.y;
	retVec.m_value.vec.z = -m_value.vec.z;

	return retVec;
}

CaptureVector3 CaptureVector3::operator+() const
{
	return *this;
}

CaptureVector3 CaptureVector3::operator-() const
{
	return  CaptureVector3(-m_value.vec.x,-m_value.vec.y,-m_value.vec.z);
}

CaptureVector3& CaptureVector3::operator+=(const CaptureVector3& v)
{
	m_value.vec.x += v.m_value.vec.x;
	m_value.vec.y += v.m_value.vec.y;
	m_value.vec.z += v.m_value.vec.z;

	return *this;
}

CaptureVector3& CaptureVector3::operator-=(const CaptureVector3& v)
{
	m_value.vec.x -= v.m_value.vec.x;
	m_value.vec.y -= v.m_value.vec.y;
	m_value.vec.z -= v.m_value.vec.z;

	return *this;
}

CaptureVector3& CaptureVector3::operator*=(const CaptureVector3& v)
{
	m_value.vec.x *= v.m_value.vec.x;
	m_value.vec.y *= v.m_value.vec.y;
	m_value.vec.z *= v.m_value.vec.z;

	return *this;
}

CaptureVector3& CaptureVector3::operator-=(float s)
{
	m_value.vec.x -= s;
	m_value.vec.y -= s;
	m_value.vec.z -= s;

	return *this;
}

CaptureVector3& CaptureVector3::operator*=(float s)
{
	m_value.vec.x *= s;
	m_value.vec.y *= s;
	m_value.vec.z *= s;

	return *this;
}

CaptureVector3& CaptureVector3::operator/=(float s)
{
	m_value.vec.x /= s;
	m_value.vec.y /= s;
	m_value.vec.z /= s;

	return *this;
}

const CaptureVector3 operator+(const CaptureVector3& v1,const CaptureVector3& v2)
{
	CaptureVector3 tmp(v1);
	return tmp += v2;
}

const CaptureVector3 operator-(const CaptureVector3& v1,const CaptureVector3& v2)
{
	CaptureVector3 tmp(v1);
	return tmp -= v2;
}

const CaptureVector3 operator-(const CaptureVector3& v,float s)
{
	CaptureVector3 tmp(v);
	return tmp -= s;
}

const CaptureVector3 operator*(const CaptureVector3& v,float s)
{
	CaptureVector3 tmp(v);
	return tmp *= s;
}

const CaptureVector3 operator*(const CaptureVector3& v,const CaptureVector3& v2)
{
	CaptureVector3 tmp(v);
	return tmp *= v2;
}

const CaptureVector3 operator*(float s,const CaptureVector3& v)
{
	return v * s;
}

const CaptureVector3 operator/(const CaptureVector3& v,float s)
{
	CaptureVector3 tmp(v);
	return tmp /= s;
}