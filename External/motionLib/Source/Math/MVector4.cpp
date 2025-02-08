#include<MVector4.h>
#include<cmath>

using namespace MCBM;

MVector4::MVector4()
{
	m_value.vec.x = 0.0f;
	m_value.vec.y = 0.0f;
	m_value.vec.z = 0.0f;
	m_value.vec.w = 0.0f;
}

MVector4::MVector4(float x,float y,float z,float w)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
	m_value.vec.w = w;
}

MVector4::MVector4(int32_t x,int32_t y,int32_t z,int32_t w)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
}

MVector4::MVector4(size_t x,size_t y,size_t z,size_t w)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
}

MVector4::MVector4(float values[ 4 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
	m_value.vec.z = values[ 2 ];
	m_value.vec.w = values[ 3 ];
}

void MVector4::Set(float value)
{
	m_value.vec.x = value;
	m_value.vec.y = value;
	m_value.vec.z = value;
	m_value.vec.w = value;
}

void MVector4::SetX(float x)
{
	m_value.vec.x = x;
}

void MVector4::SetX(size_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void MVector4::SetX(int32_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void MVector4::SetY(float y)
{
	m_value.vec.y = y;
}

void MVector4::SetY(size_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void MVector4::SetY(int32_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void MVector4::SetZ(float z)
{
	m_value.vec.z = z;
}

void MVector4::SetZ(size_t z)
{
	m_value.vec.z = static_cast< float >( z );
}

void MVector4::SetZ(int32_t z)
{
	m_value.vec.z = static_cast< float >( z );
}

void MVector4::SetW(float w)
{
	m_value.vec.w = w;
}

void MVector4::SetW(size_t w)
{
	m_value.vec.w = static_cast< float >( w );
}

void MVector4::SetW(int32_t w)
{
	m_value.vec.w = static_cast< float >( w );
}

void MVector4::SetValue(float x,float y,float z,float w)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
	m_value.vec.w = w;
}

void MVector4::SetValue(size_t x,size_t y,size_t z,size_t w)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
}

void MVector4::SetValue(int32_t x,int32_t y,int32_t z,size_t w)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
	m_value.vec.z = static_cast< float >( z );
	m_value.vec.w = static_cast< float >( w );
}

void MVector4::SetValues(float values[ 4 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
	m_value.vec.z = values[ 2 ];
	m_value.vec.w = values[ 3 ];
}

void MVector4::SetValues(size_t values[ 4 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
	m_value.vec.z = static_cast< float >( values[ 2 ] );
	m_value.vec.w = static_cast< float >( values[ 3 ] );
}

void MVector4::SetValues(int32_t values[ 4 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
	m_value.vec.z = static_cast< float >( values[ 2 ] );
	m_value.vec.w = static_cast< float >( values[ 3 ] );
}

float MVector4::GetX() const
{
	return m_value.vec.x;
}

float MVector4::GetY() const
{
	return m_value.vec.y;
}

float MVector4::GetZ() const
{
	return m_value.vec.z;
}

float MVector4::GetW() const
{
	return m_value.vec.w;
}

const float* MVector4::GetValues() const
{
	return m_value.elements;
}

float MVector4::Length() const
{
	return std::sqrt(m_value.vec.x * m_value.vec.x + m_value.vec.y * m_value.vec.y + m_value.vec.z * m_value.vec.z + m_value.vec.w * m_value.vec.w);
}

MVector4 MVector4::Normalization() const
{
	MVector4 tmp(*this);

	float len = Length();

	if ( len != 0 )
	{
		return tmp /= len;

	}

	return tmp;
}

MVector4& MVector4::Normalization()
{
	float len = Length();

	if ( len != 0 )
	{
		return *this /= len;
	}

	return *this;
}

float MVector4::Dot(const MVector4& v) const
{
	return v.m_value.vec.x * m_value.vec.x + v.m_value.vec.y * m_value.vec.y + v.m_value.vec.z * m_value.vec.z + v.m_value.vec.w * m_value.vec.w;
}

MVector4 MVector4::Cross(const MVector4& v1,const MVector4& v2) const
{
	MVector4 tmp;

	tmp.m_value.vec.x = m_value.vec.y * ( v1.m_value.vec.z * v2.m_value.vec.w - v2.m_value.vec.z * v1.m_value.vec.w ) - m_value.vec.z * ( v1.m_value.vec.y * v2.m_value.vec.w - v2.m_value.vec.y * v1.m_value.vec.w ) + m_value.vec.w * ( v1.m_value.vec.y * v2.m_value.vec.z - v2.m_value.vec.y * v1.m_value.vec.z );

	tmp.m_value.vec.y = m_value.vec.x * ( v2.m_value.vec.z * v1.m_value.vec.w - v1.m_value.vec.z * v2.m_value.vec.w ) - m_value.vec.z * ( v2.m_value.vec.x * v1.m_value.vec.w - v1.m_value.vec.x * v2.m_value.vec.w ) + m_value.vec.w * ( v2.m_value.vec.x * v1.m_value.vec.z - v1.m_value.vec.x * v2.m_value.vec.z );

	tmp.m_value.vec.z = m_value.vec.x * ( v1.m_value.vec.y * v2.m_value.vec.w - v2.m_value.vec.y * v1.m_value.vec.w ) - m_value.vec.y * ( v1.m_value.vec.x * v2.m_value.vec.w - v2.m_value.vec.x * v1.m_value.vec.w ) + m_value.vec.w * ( v1.m_value.vec.x * v2.m_value.vec.y - v2.m_value.vec.x * v1.m_value.vec.y );

	tmp.m_value.vec.w = m_value.vec.x * ( v2.m_value.vec.y * v1.m_value.vec.z - v1.m_value.vec.y * v2.m_value.vec.z ) - m_value.vec.y * ( v2.m_value.vec.x * v1.m_value.vec.z - v1.m_value.vec.x * v2.m_value.vec.z ) + m_value.vec.z * ( v2.m_value.vec.x * v1.m_value.vec.y - v1.m_value.vec.x * v2.m_value.vec.y );

	return tmp;

}
float MVector4::Vector3Length() const
{
	return std::sqrt(m_value.vec.x * m_value.vec.x + m_value.vec.y * m_value.vec.y + m_value.vec.z * m_value.vec.z);
}

MVector4 MVector4::Vector3Normalization() const
{
	MVector4 tmp(*this);

	float len = Vector3Length();

	if ( len != 0 )
	{
		return tmp /= len;

	}

	return tmp;
}

MVector4& MVector4::Vector3Normalization()
{
	float len = Vector3Length();

	if ( len != 0 )
	{
		return *this /= len;

	}

	return *this;
}

float MVector4::Vector3Dot(const MVector4& v) const
{
	return m_value.vec.x * v.m_value.vec.x + m_value.vec.y * v.m_value.vec.y + m_value.vec.z * v.m_value.vec.z;
}

MVector4 MVector4::Vector3Cross(const MVector4& v) const
{
	MVector4 tmp;

	tmp.m_value.vec.x = m_value.vec.y * v.m_value.vec.z - m_value.vec.z * v.m_value.vec.y;
	tmp.m_value.vec.y = m_value.vec.z * v.m_value.vec.x - m_value.vec.x * v.m_value.vec.z;
	tmp.m_value.vec.z = m_value.vec.x * v.m_value.vec.y - m_value.vec.y * v.m_value.vec.x;
	tmp.m_value.vec.w = 0.0f;

	return tmp;
}

MVector4 MVector4::operator+() const
{
	return *this;
}

MVector4 MVector4::operator-() const
{
	return  MVector4(-m_value.vec.x,-m_value.vec.y,-m_value.vec.z,-m_value.vec.w);
}

MVector4& MVector4::operator+=(const MVector4& v)
{
	m_value.vec.x += v.m_value.vec.x;
	m_value.vec.y += v.m_value.vec.y;
	m_value.vec.z += v.m_value.vec.z;
	m_value.vec.w += v.m_value.vec.w;

	return *this;
}

MVector4& MVector4::operator-=(const MVector4& v)
{
	m_value.vec.x -= v.m_value.vec.x;
	m_value.vec.y -= v.m_value.vec.y;
	m_value.vec.z -= v.m_value.vec.z;
	m_value.vec.w -= v.m_value.vec.w;

	return *this;
}

MVector4& MVector4::operator*=(float s)
{
	m_value.vec.x *= s;
	m_value.vec.y *= s;
	m_value.vec.z *= s;
	m_value.vec.w *= s;

	return *this;
}

MVector4& MVector4::operator/=(float s)
{
	m_value.vec.x /= s;
	m_value.vec.y /= s;
	m_value.vec.z /= s;
	m_value.vec.w /= s;

	return *this;
}

const MVector4 operator+(const MVector4& v1,const MVector4& v2)
{
	MVector4 tmp(v1);
	return tmp += v2;
}

const MVector4 operator-(const MVector4& v1,const MVector4& v2)
{
	MVector4 tmp(v1);
	return tmp -= v2;
}

const MVector4 operator*(const MVector4& v,float s)
{
	MVector4 tmp(v);
	return tmp *= s;
}

const MVector4 operator*(float s, const MVector4& v)
{
	MVector4 tmp(v);
	return tmp *= s;
}

const MVector4 operator/(const MVector4& v,float s)
{
	MVector4 tmp(v);
	return tmp /= s;
}