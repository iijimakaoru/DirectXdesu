#include<PVector4.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cmath>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

PVector4::PVector4()
{
	m_value.vec.x = 0.0f;
	m_value.vec.y = 0.0f;
	m_value.vec.z = 0.0f;
	m_value.vec.w = 0.0f;
}

PVector4::PVector4(float x,float y,float z,float w)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
	m_value.vec.w = w;
}

PVector4::PVector4(int32_t x,int32_t y,int32_t z,int32_t w)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
}

PVector4::PVector4(size_t x,size_t y,size_t z,size_t w)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
}

PVector4::PVector4(float values[ 4 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
	m_value.vec.z = values[ 2 ];
	m_value.vec.w = values[ 3 ];
}

void PVector4::Set(float value)
{
	m_value.vec.x = value;
	m_value.vec.y = value;
	m_value.vec.z = value;
	m_value.vec.w = value;
}

void PVector4::SetX(float x)
{
	m_value.vec.x = x;
}

void PVector4::SetX(size_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void PVector4::SetX(int32_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void PVector4::SetY(float y)
{
	m_value.vec.y = y;
}

void PVector4::SetY(size_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void PVector4::SetY(int32_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void PVector4::SetZ(float z)
{
	m_value.vec.z = z;
}

void PVector4::SetZ(size_t z)
{
	m_value.vec.z = static_cast< float >( z );
}

void PVector4::SetZ(int32_t z)
{
	m_value.vec.z = static_cast< float >( z );
}

void PVector4::SetW(float w)
{
	m_value.vec.w = w;
}

void PVector4::SetW(size_t w)
{
	m_value.vec.w = static_cast< float >( w );
}

void PVector4::SetW(int32_t w)
{
	m_value.vec.w = static_cast< float >( w );
}

void PVector4::SetValue(float x,float y,float z,float w)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
	m_value.vec.w = w;
}

void PVector4::SetValue(size_t x,size_t y,size_t z,size_t w)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
}

void PVector4::SetValue(int32_t x,int32_t y,int32_t z,size_t w)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
}

void PVector4::SetValues(float values[ 4 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
	m_value.vec.z = values[ 2 ];
	m_value.vec.w = values[ 3 ];
}

void PVector4::SetValues(size_t values[ 4 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
	m_value.vec.z = static_cast< float >( values[ 2 ] );
	m_value.vec.w = static_cast< float >( values[ 3 ] );
}

void PVector4::SetValues(int32_t values[ 4 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
	m_value.vec.z = static_cast< float >( values[ 2 ] );
	m_value.vec.w = static_cast< float >( values[ 3 ] );
}

float PVector4::GetX() const
{
	return m_value.vec.x;
}

float PVector4::GetY() const
{
	return m_value.vec.y;
}

float PVector4::GetZ() const
{
	return m_value.vec.z;
}

float PVector4::GetW() const
{
	return m_value.vec.w;
}

const float* PVector4::GetValues() const
{
	return m_value.elements;
}

float PVector4::Length() const
{
	return std::sqrt(m_value.vec.x * m_value.vec.x + m_value.vec.y * m_value.vec.y + m_value.vec.z * m_value.vec.z + m_value.vec.w * m_value.vec.w);
}

PVector4 PVector4::Normalization() const
{
	PVector4 tmp(*this);

	float len = Length();

	if ( len != 0 )
	{
		return tmp /= len;

	}

	return tmp;
}

PVector4& PVector4::Normalization()
{
	float len = Length();

	if ( len != 0 )
	{
		return *this /= len;
	}

	return *this;
}

float PVector4::Dot(const PVector4& v) const
{
	return v.m_value.vec.x * m_value.vec.x + v.m_value.vec.y * m_value.vec.y + v.m_value.vec.z * m_value.vec.z + v.m_value.vec.w * m_value.vec.w;
}

PVector4 PVector4::Cross(const PVector4& v1,const PVector4& v2) const
{
	PVector4 tmp;

	tmp.m_value.vec.x = m_value.vec.y * ( v1.m_value.vec.z * v2.m_value.vec.w - v2.m_value.vec.z * v1.m_value.vec.w ) - m_value.vec.z * ( v1.m_value.vec.y * v2.m_value.vec.w - v2.m_value.vec.y * v1.m_value.vec.w ) + m_value.vec.w * ( v1.m_value.vec.y * v2.m_value.vec.z - v2.m_value.vec.y * v1.m_value.vec.z );

	tmp.m_value.vec.y = m_value.vec.x * ( v2.m_value.vec.z * v1.m_value.vec.w - v1.m_value.vec.z * v2.m_value.vec.w ) - m_value.vec.z * ( v2.m_value.vec.x * v1.m_value.vec.w - v1.m_value.vec.x * v2.m_value.vec.w ) + m_value.vec.w * ( v2.m_value.vec.x * v1.m_value.vec.z - v1.m_value.vec.x * v2.m_value.vec.z );

	tmp.m_value.vec.z = m_value.vec.x * ( v1.m_value.vec.y * v2.m_value.vec.w - v2.m_value.vec.y * v1.m_value.vec.w ) - m_value.vec.y * ( v1.m_value.vec.x * v2.m_value.vec.w - v2.m_value.vec.x * v1.m_value.vec.w ) + m_value.vec.w * ( v1.m_value.vec.x * v2.m_value.vec.y - v2.m_value.vec.x * v1.m_value.vec.y );

	tmp.m_value.vec.w = m_value.vec.x * ( v2.m_value.vec.y * v1.m_value.vec.z - v1.m_value.vec.y * v2.m_value.vec.z ) - m_value.vec.y * ( v2.m_value.vec.x * v1.m_value.vec.z - v1.m_value.vec.x * v2.m_value.vec.z ) + m_value.vec.z * ( v2.m_value.vec.x * v1.m_value.vec.y - v1.m_value.vec.x * v2.m_value.vec.y );

	return tmp;

}
float PVector4::Vector3Length() const
{
	return std::sqrt(m_value.vec.x * m_value.vec.x + m_value.vec.y * m_value.vec.y + m_value.vec.z * m_value.vec.z);
}

PVector4 PVector4::Vector3Normalization() const
{
	PVector4 tmp(*this);

	float len = Vector3Length();

	if ( len != 0 )
	{
		return tmp /= len;

	}

	return tmp;
}

PVector4& PVector4::Vector3Normalization()
{
	float len = Vector3Length();

	if ( len != 0 )
	{
		return *this /= len;

	}

	return *this;
}

float PVector4::Vector3Dot(const PVector4& v) const
{
	return m_value.vec.x * v.m_value.vec.x + m_value.vec.y * v.m_value.vec.y + m_value.vec.z * v.m_value.vec.z;
}

PVector4 PVector4::Vector3Cross(const PVector4& v) const
{
	PVector4 tmp;

	tmp.m_value.vec.x = m_value.vec.y * v.m_value.vec.z - m_value.vec.z * v.m_value.vec.y;
	tmp.m_value.vec.y = m_value.vec.z * v.m_value.vec.x - m_value.vec.x * v.m_value.vec.z;
	tmp.m_value.vec.z = m_value.vec.x * v.m_value.vec.y - m_value.vec.y * v.m_value.vec.x;
	tmp.m_value.vec.w = 0.0f;

	return tmp;
}

PVector4 PVector4::operator+() const
{
	return *this;
}

PVector4 PVector4::operator-() const
{
	return  PVector4(-m_value.vec.x,-m_value.vec.y,-m_value.vec.z,-m_value.vec.w);
}

PVector4& PVector4::operator+=(const PVector4& v)
{
	m_value.vec.x += v.m_value.vec.x;
	m_value.vec.y += v.m_value.vec.y;
	m_value.vec.z += v.m_value.vec.z;
	m_value.vec.w += v.m_value.vec.w;

	return *this;
}

PVector4& PVector4::operator-=(const PVector4& v)
{
	m_value.vec.x -= v.m_value.vec.x;
	m_value.vec.y -= v.m_value.vec.y;
	m_value.vec.z -= v.m_value.vec.z;
	m_value.vec.w -= v.m_value.vec.w;

	return *this;
}

PVector4& PVector4::operator*=(float s)
{
	m_value.vec.x *= s;
	m_value.vec.y *= s;
	m_value.vec.z *= s;
	m_value.vec.w *= s;

	return *this;
}

PVector4& PVector4::operator/=(float s)
{
	m_value.vec.x /= s;
	m_value.vec.y /= s;
	m_value.vec.z /= s;
	m_value.vec.w /= s;

	return *this;
}

const PVector4 operator+(const PVector4& v1,const PVector4& v2)
{
	PVector4 tmp(v1);
	return tmp += v2;
}

const PVector4 operator-(const PVector4& v1,const PVector4& v2)
{
	PVector4 tmp(v1);
	return tmp -= v2;
}

const PVector4 operator*(const PVector4& v,float s)
{
	PVector4 tmp(v);
	return tmp *= s;
}

const PVector4 operator*(float s,const PVector4& v)
{
	return v * s;
}

const PVector4 operator/(const PVector4& v,float s)
{
	PVector4 tmp(v);
	return tmp /= s;
}

PHONON_LOADER_NAMESPACE_END