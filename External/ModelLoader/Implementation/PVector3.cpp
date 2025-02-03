#include <PVector3.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cmath>
#include<algorithm>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

PVector3::PVector3()
{
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
}

PVector3::PVector3(float x,float y,float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

PVector3::PVector3(int32_t x,int32_t y,int32_t z)
{
	m_x = static_cast< float > ( x );
	m_y = static_cast< float > ( y );
	m_z = static_cast< float > ( z );
}

PVector3::PVector3(size_t x,size_t y,size_t z)
{
	m_x = static_cast< float > ( x );
	m_y = static_cast< float > ( y );
	m_z = static_cast< float > ( z );
}

PVector3::PVector3(float values[ 3 ])
{
	m_x = values[0];
	m_y = values[1];
	m_z = values[2];
}

void PVector3::SetX(float x)
{
	m_x = x;
}

void PVector3::SetX(size_t x)
{
	m_x = static_cast< float >( x );
}

void PVector3::SetX(int32_t x)
{
	m_x = static_cast< float >( x );
}

void PVector3::SetY(float y)
{
	m_y = y;
}

void PVector3::SetY(size_t y)
{
	m_y = static_cast< float >( y );
}

void PVector3::SetY(int32_t y)
{
	m_y = static_cast< float >( y );
}

void PVector3::SetZ(float z)
{
	m_z = z;
}

void PVector3::SetZ(size_t z)
{
	m_z = static_cast< float >( z );
}

void PVector3::SetZ(int32_t z)
{
	m_z = static_cast< float >( z );
}

void PVector3::SetValue(float x,float y,float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void PVector3::SetValue(size_t x,size_t y,size_t z)
{
	m_x = static_cast< float > ( x );
	m_y = static_cast< float > ( y );
	m_z = static_cast< float > ( z );
}

void PVector3::SetValue(int32_t x,int32_t y,int32_t z)
{
	m_x = static_cast< float > ( x );
	m_y = static_cast< float > ( y );
	m_z = static_cast< float > ( z );
}

void PVector3::SetValues(float values[ 3 ])
{
	m_x = values[ 0 ];
	m_y = values[ 1 ];
	m_z = values[ 2 ];
}

void PVector3::SetValues(size_t values[ 3 ])
{
	m_x = static_cast< float > ( values[ 0 ] );
	m_y = static_cast< float > ( values[ 1 ] );
	m_z = static_cast< float > ( values[ 2 ] );
}

void PVector3::SetValues(int32_t values[ 3 ])
{
	m_x = static_cast< float > ( values[ 0 ] );
	m_y = static_cast< float > ( values[ 1 ] );
	m_z = static_cast< float > ( values[ 2 ] );
}

float PVector3::GetX() const
{
	return m_x;
}

float PVector3::GetY() const
{
	return m_y;
}

float PVector3::GetZ() const
{
	return m_z;
}

const float* PVector3::GetValues() const
{
	return m_values;
}


float PVector3::Length() const
{
	return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

PVector3& PVector3::Normalization()
{
	float len = Length();

	if ( len != 0 )
	{
		return *this /= len;
	}

	return *this;
}

PVector3 PVector3::Normalization() const
{
	PVector3 tmp(*this);

	float len = Length();

	if ( len != 0 )
	{
		return tmp /= len;
	}

	return tmp;
}

float PVector3::Dot(const PVector3& v) const
{
	return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
}

PVector3 PVector3::Cross(const PVector3& v) const
{
	PVector3 w;

	w.m_x = m_y * v.m_z - m_z * v.m_y;
	w.m_y = m_z * v.m_x - m_x * v.m_z;
	w.m_z = m_x * v.m_y - m_y * v.m_x;

	return w;
}

PVector3 PVector3::Negate() const
{
	PVector3 retVec;
	retVec.m_x = -m_x;
	retVec.m_y = -m_y;
	retVec.m_z = -m_z;

	return retVec;
}

PVector3 PVector3::operator+() const
{
	return *this;
}

PVector3 PVector3::operator-() const
{
	return  PVector3(-m_x,-m_y,-m_z);
}

PVector3& PVector3::operator+=(const PVector3& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
	m_z += v.m_z;

	return *this;
}

PVector3& PVector3::operator-=(const PVector3& v)
{
	m_x -= v.m_x;
	m_y -= v.m_y;
	m_z -= v.m_z;

	return *this;
}

PVector3& PVector3::operator*=(const PVector3& v)
{
	m_x *= v.m_x;
	m_y *= v.m_y;
	m_z *= v.m_z;

	return *this;
}

PVector3& PVector3::operator-=(float s)
{
	m_x -= s;
	m_y -= s;
	m_z -= s;

	return *this;
}

PVector3& PVector3::operator*=(float s)
{
	m_x *= s;
	m_y *= s;
	m_z *= s;

	return *this;
}

PVector3& PVector3::operator/=(float s)
{
	m_x /= s;
	m_y /= s;
	m_z /= s;

	return *this;
}

bool PVector3::operator==(const PVector3& v) const
{
	if ( _Approximately(this->m_x,v.m_x) && _Approximately(this->m_y,v.m_y) && _Approximately(this->m_z,v.m_z) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PVector3::operator!=(const PVector3& v) const
{
	if ( _Approximately(this->m_x,v.m_x) && _Approximately(this->m_y,v.m_y) && _Approximately(this->m_z,v.m_z) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

const PVector3 operator+(const PVector3& v1,const PVector3& v2)
{
	PVector3 tmp(v1);
	return tmp += v2;
}

const PVector3 operator-(const PVector3& v1,const PVector3& v2)
{
	PVector3 tmp(v1);
	return tmp -= v2;
}

const PVector3 operator-(const PVector3& v,float s)
{
	PVector3 tmp(v);
	return tmp -= s;
}

const PVector3 operator*(const PVector3& v,float s)
{
	PVector3 tmp(v);
	return tmp *= s;
}

const PVector3 operator*(const PVector3& v,const PVector3& v2)
{
	PVector3 tmp(v);
	return tmp *= v2;
}

const PVector3 operator*(float s,const PVector3& v)
{
	return v * s;
}

const PVector3 operator/(const PVector3& v,float s)
{
	PVector3 tmp(v);
	return tmp /= s;
}

PHONON_LOADER_NAMESPACE_END