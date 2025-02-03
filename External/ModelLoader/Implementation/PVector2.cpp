#include <PVector2.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cmath>
#include<algorithm>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

PVector2::PVector2()
{
	m_value.vec.x = 0.0f;
	m_value.vec.y = 0.0f;
}

PVector2::PVector2(float x,float y)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
}

PVector2::PVector2(size_t x,size_t y)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
}

PVector2::PVector2(int32_t x,int32_t y)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
}

void PVector2::SetX(float x)
{
	m_value.vec.x = x;
}

void PVector2::SetX(size_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void PVector2::SetX(int32_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void PVector2::SetY(float y)
{
	m_value.vec.y = y;
}

void PVector2::SetY(size_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void PVector2::SetY(int32_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void PVector2::SetValue(float x,float y)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
}

void PVector2::SetValue(size_t x,size_t y)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
}

void PVector2::SetValue(int32_t x,int32_t y)
{
	m_value.vec.x = static_cast< float >( y );
	m_value.vec.y = static_cast< float >( x );
}

void PVector2::SetValues(float values[ 2 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
}

void PVector2::SetValues(size_t values[ 2 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
}

void PVector2::SetValues(int32_t values[ 2 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
}

float PVector2::GetX() const
{
	return m_value.vec.x;
}

float PVector2::GetY() const
{
	return m_value.vec.y;
}

const float* PVector2::GetValues() const
{
	return m_value.elements;
}

float PVector2::Length() const
{
	return std::sqrtf(m_value.vec.x * m_value.vec.x + m_value.vec.y * m_value.vec.y);
}

PVector2& PVector2::Normalization()
{
	float lLen = Length();

	if ( lLen != 0 )
	{
		return *this /= lLen;

	}

	return *this;
}

float PVector2::Dot(const PVector2& v_) const
{
	return m_value.vec.x * v_.m_value.vec.x + m_value.vec.y * v_.m_value.vec.y;
}

float PVector2::Cross(const PVector2& v_) const
{
	return m_value.vec.x * v_.m_value.vec.y - v_.m_value.vec.x * m_value.vec.y;
}

PVector2 PVector2::operator+() const
{
	return *this;
}

PVector2 PVector2::operator-() const
{
	return  PVector2(-m_value.vec.x,-m_value.vec.y);
}

PVector2& PVector2::operator+=(const PVector2& v_)
{
	m_value.vec.x += v_.m_value.vec.x;
	m_value.vec.y += v_.m_value.vec.y;

	return *this;
}

PVector2& PVector2::operator-=(const PVector2& v_)
{
	m_value.vec.x -= v_.m_value.vec.x;
	m_value.vec.y -= v_.m_value.vec.y;

	return *this;
}

PVector2& PVector2::operator*=(float s_)
{
	m_value.vec.x *= s_;
	m_value.vec.y *= s_;

	return *this;
}

PVector2& PVector2::operator/=(float s_)
{
	m_value.vec.x /= s_;
	m_value.vec.y /= s_;

	return *this;
}

PVector2& PVector2::operator/=(const PVector2& v_)
{
	m_value.vec.x /= v_.m_value.vec.x;
	m_value.vec.y /= v_.m_value.vec.y;
	return *this;
}

bool PVector2::operator==(const PVector2& v_)
{
	if ( _Approximately(v_.m_value.vec.x,m_value.vec.x) && _Approximately(v_.m_value.vec.y,m_value.vec.y) )
	{
		return true;
	}

	return false;
}

const PVector2 operator+(const PVector2& v1_,const PVector2& v2_)
{
	PVector2 lTmp(v1_);
	return lTmp += v2_;
}

const PVector2 operator-(const PVector2& v1_,const PVector2& v2_)
{
	PVector2 lTmp(v1_);
	return lTmp -= v2_;
}

const PVector2 operator*(const PVector2& v_,float s_)
{
	PVector2 lTmp(v_);
	return lTmp *= s_;
}

const PVector2 operator*(float s_,const PVector2& v_)
{
	return v_ * s_;
}

const PVector2 operator/(const PVector2& v_,float s_)
{
	PVector2 lTmp(v_);
	return lTmp /= s_;
}

PHONON_LOADER_NAMESPACE_END