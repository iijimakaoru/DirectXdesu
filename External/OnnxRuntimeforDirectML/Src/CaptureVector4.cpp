#include<CaptureVector4.h>

#include<cmath>

CaptureVector5::CaptureVector5()
{
	m_value.vec.x = 0.0f;
	m_value.vec.y = 0.0f;
	m_value.vec.z = 0.0f;
	m_value.vec.w = 0.0f;
}

CaptureVector5::CaptureVector5(float x,float y,float z,float w,float v)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
	m_value.vec.w = w;
	m_value.vec.v = v;
}

CaptureVector5::CaptureVector5(int32_t x,int32_t y,int32_t z,int32_t w,int32_t v)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
	m_value.vec.v = static_cast< float >( v );
}

CaptureVector5::CaptureVector5(size_t x,size_t y,size_t z,size_t w,size_t v)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
	m_value.vec.v = static_cast< float >( v );
}

CaptureVector5::CaptureVector5(float values[ 5 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
	m_value.vec.z = values[ 2 ];
	m_value.vec.w = values[ 3 ];
	m_value.vec.w = values[ 4 ];
}

void CaptureVector5::Set(float value)
{
	m_value.vec.x = value;
	m_value.vec.y = value;
	m_value.vec.z = value;
	m_value.vec.w = value;
	m_value.vec.v = value;
}

void CaptureVector5::Set(size_t index,float value)
{
	m_value.elements[index] = value;
}

void CaptureVector5::SetX(float x)
{
	m_value.vec.x = x;
}

void CaptureVector5::SetX(size_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void CaptureVector5::SetX(int32_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void CaptureVector5::SetY(float y)
{
	m_value.vec.y = y;
}

void CaptureVector5::SetY(size_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void CaptureVector5::SetY(int32_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void CaptureVector5::SetZ(float z)
{
	m_value.vec.z = z;
}

void CaptureVector5::SetZ(size_t z)
{
	m_value.vec.z = static_cast< float >( z );
}

void CaptureVector5::SetZ(int32_t z)
{
	m_value.vec.z = static_cast< float >( z );
}

void CaptureVector5::SetW(float w)
{
	m_value.vec.w = w;
}

void CaptureVector5::SetW(size_t w)
{
	m_value.vec.w = static_cast< float >( w );
}

void CaptureVector5::SetW(int32_t w)
{
	m_value.vec.w = static_cast< float >( w );
}

void CaptureVector5::SetV(float v)
{
	m_value.vec.v = v;
}

void CaptureVector5::SetV(size_t v)
{
	m_value.vec.v = static_cast< float >( v );
}

void CaptureVector5::SetV(int32_t v)
{
	m_value.vec.v = static_cast< float >( v );
}

void CaptureVector5::SetValue(float x,float y,float z,float w,float v)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
	m_value.vec.w = w;
	m_value.vec.v = v;
}

void CaptureVector5::SetValue(size_t x,size_t y,size_t z,size_t w,size_t v)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
	m_value.vec.v = static_cast< float >( v );
}

void CaptureVector5::SetValue(int32_t x,int32_t y,int32_t z,int32_t w,int32_t v)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
	m_value.vec.v = static_cast< float >( v );
}

void CaptureVector5::SetValues(float values[ 5 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
	m_value.vec.z = values[ 2 ];
	m_value.vec.w = values[ 3 ];
	m_value.vec.v = values[ 4 ];
}

void CaptureVector5::SetValues(size_t values[ 5 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
	m_value.vec.z = static_cast< float >( values[ 2 ] );
	m_value.vec.w = static_cast< float >( values[ 3 ] );
	m_value.vec.v = static_cast< float >( values[ 4 ] );
}

void CaptureVector5::SetValues(int32_t values[ 5 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
	m_value.vec.z = static_cast< float >( values[ 2 ] );
	m_value.vec.w = static_cast< float >( values[ 3 ] );
	m_value.vec.v = static_cast< float >( values[ 4 ] );
}

float CaptureVector5::GetX() const
{
	return m_value.vec.x;
}

float CaptureVector5::GetY() const
{
	return m_value.vec.y;
}

float CaptureVector5::GetZ() const
{
	return m_value.vec.z;
}

float CaptureVector5::GetW() const
{
	return m_value.vec.w;
}

float CaptureVector5::GetV() const
{
	return m_value.vec.v;
}

const float* CaptureVector5::GetValues() const
{
	return m_value.elements;
}

float CaptureVector5::Length() const
{
	return std::sqrt(m_value.vec.x * m_value.vec.x + m_value.vec.y * m_value.vec.y + m_value.vec.z * m_value.vec.z + m_value.vec.w * m_value.vec.w + m_value.vec.v * m_value.vec.v);
}

CaptureVector5 CaptureVector5::Normalization() const
{
	CaptureVector5 tmp(*this);

	float len = Length();

	if ( len != 0 )
	{
		return tmp /= len;

	}

	return tmp;
}

CaptureVector5& CaptureVector5::Normalization()
{
	float len = Length();

	if ( len != 0 )
	{
		return *this /= len;
	}

	return *this;
}

float CaptureVector5::Dot(const CaptureVector5& v) const
{
	return v.m_value.vec.x * m_value.vec.x + v.m_value.vec.y * m_value.vec.y + v.m_value.vec.z * m_value.vec.z + v.m_value.vec.w * m_value.vec.w + m_value.vec.v * m_value.vec.v;
}

CaptureVector5 CaptureVector5::Cross(const CaptureVector5& v1,const CaptureVector5& v2) const
{
	CaptureVector5 tmp;

	tmp.m_value.vec.x = m_value.vec.y * ( v1.m_value.vec.z * v2.m_value.vec.w - v2.m_value.vec.z * v1.m_value.vec.w )
		- m_value.vec.z * ( v1.m_value.vec.y * v2.m_value.vec.w - v2.m_value.vec.y * v1.m_value.vec.w )
		+ m_value.vec.w * ( v1.m_value.vec.y * v2.m_value.vec.z - v2.m_value.vec.y * v1.m_value.vec.z )
		- m_value.vec.v * ( v1.m_value.vec.y * v2.m_value.vec.z - v2.m_value.vec.y * v1.m_value.vec.z );

	tmp.m_value.vec.y = m_value.vec.x * ( v2.m_value.vec.z * v1.m_value.vec.w - v1.m_value.vec.z * v2.m_value.vec.w )
		- m_value.vec.z * ( v2.m_value.vec.x * v1.m_value.vec.w - v1.m_value.vec.x * v2.m_value.vec.w )
		+ m_value.vec.w * ( v2.m_value.vec.x * v1.m_value.vec.z - v1.m_value.vec.x * v2.m_value.vec.z )
		- m_value.vec.v * ( v2.m_value.vec.x * v1.m_value.vec.z - v1.m_value.vec.x * v2.m_value.vec.z );

	tmp.m_value.vec.z = m_value.vec.x * ( v1.m_value.vec.y * v2.m_value.vec.w - v2.m_value.vec.y * v1.m_value.vec.w )
		- m_value.vec.y * ( v1.m_value.vec.x * v2.m_value.vec.w - v2.m_value.vec.x * v1.m_value.vec.w )
		+ m_value.vec.w * ( v1.m_value.vec.x * v2.m_value.vec.y - v2.m_value.vec.x * v1.m_value.vec.y )
		- m_value.vec.v * ( v1.m_value.vec.x * v2.m_value.vec.y - v2.m_value.vec.x * v1.m_value.vec.y );

	tmp.m_value.vec.w = m_value.vec.x * ( v2.m_value.vec.y * v1.m_value.vec.z - v1.m_value.vec.y * v2.m_value.vec.z )
		- m_value.vec.y * ( v2.m_value.vec.x * v1.m_value.vec.z - v1.m_value.vec.x * v2.m_value.vec.z )
		+ m_value.vec.z * ( v2.m_value.vec.x * v1.m_value.vec.y - v1.m_value.vec.x * v2.m_value.vec.y )
		- m_value.vec.v * ( v2.m_value.vec.x * v1.m_value.vec.y - v1.m_value.vec.x * v2.m_value.vec.y );

	tmp.m_value.vec.v = m_value.vec.x * ( v1.m_value.vec.y * v2.m_value.vec.z - v2.m_value.vec.y * v1.m_value.vec.z )
		- m_value.vec.y * ( v1.m_value.vec.x * v2.m_value.vec.z - v2.m_value.vec.x * v1.m_value.vec.z )
		+ m_value.vec.z * ( v1.m_value.vec.x * v2.m_value.vec.y - v2.m_value.vec.x * v1.m_value.vec.y )
		- m_value.vec.w * ( v1.m_value.vec.x * v2.m_value.vec.y - v2.m_value.vec.x * v1.m_value.vec.y );

	return tmp;
}


CaptureVector5 CaptureVector5::operator+() const
{
	return *this;
}

CaptureVector5 CaptureVector5::operator-() const
{
	return  CaptureVector5(-m_value.vec.x,-m_value.vec.y,-m_value.vec.z,-m_value.vec.w,-m_value.vec.v);
}

CaptureVector5& CaptureVector5::operator+=(const CaptureVector5& v)
{
	m_value.vec.x += v.m_value.vec.x;
	m_value.vec.y += v.m_value.vec.y;
	m_value.vec.z += v.m_value.vec.z;
	m_value.vec.w += v.m_value.vec.w;
	m_value.vec.v += v.m_value.vec.v;

	return *this;
}

CaptureVector5& CaptureVector5::operator-=(const CaptureVector5& v)
{
	m_value.vec.x -= v.m_value.vec.x;
	m_value.vec.y -= v.m_value.vec.y;
	m_value.vec.z -= v.m_value.vec.z;
	m_value.vec.w -= v.m_value.vec.w;
	m_value.vec.v -= v.m_value.vec.v;

	return *this;
}

CaptureVector5& CaptureVector5::operator*=(float s)
{
	m_value.vec.x *= s;
	m_value.vec.y *= s;
	m_value.vec.z *= s;
	m_value.vec.w *= s;
	m_value.vec.v *= s;

	return *this;
}

CaptureVector5& CaptureVector5::operator/=(float s)
{
	m_value.vec.x /= s;
	m_value.vec.y /= s;
	m_value.vec.z /= s;
	m_value.vec.w /= s;
	m_value.vec.v /= s;

	return *this;
}

const CaptureVector5 operator+(const CaptureVector5& v1,const CaptureVector5& v2)
{
	CaptureVector5 tmp(v1);
	return tmp += v2;
}

const CaptureVector5 operator-(const CaptureVector5& v1,const CaptureVector5& v2)
{
	CaptureVector5 tmp(v1);
	return tmp -= v2;
}

const CaptureVector5 operator*(const CaptureVector5& v,float s)
{
	CaptureVector5 tmp(v);
	return tmp *= s;
}

const CaptureVector5 operator*(float s,const CaptureVector5& v)
{
	return v * s;
}

const CaptureVector5 operator/(const CaptureVector5& v,float s)
{
	CaptureVector5 tmp(v);
	return tmp /= s;
}