#include <MVector2.h>
#include<cmath>
#include<algorithm>

using namespace MCBM;

MVector2::MVector2()
{
	m_value.vec.x = 0.0f;
	m_value.vec.y = 0.0f;
}

MVector2::MVector2(float x,float y)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
}

MVector2::MVector2(size_t x,size_t y)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
}

MVector2::MVector2(int32_t x,int32_t y)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
}

void MVector2::SetX(float x)
{
	m_value.vec.x = x;
}

void MVector2::SetX(size_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void MVector2::SetX(int32_t x)
{
	m_value.vec.x = static_cast< float >( x );
}

void MVector2::SetY(float y)
{
	m_value.vec.y = y;
}

void MVector2::SetY(size_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void MVector2::SetY(int32_t y)
{
	m_value.vec.y = static_cast< float >( y );
}

void MVector2::SetValue(float x,float y)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
}

void MVector2::SetValue(size_t x,size_t y)
{
	m_value.vec.x = static_cast< float >( x );
	m_value.vec.y = static_cast< float >( y );
}

void MVector2::SetValue(int32_t x,int32_t y)
{
	m_value.vec.x = static_cast< float >( y );
	m_value.vec.y = static_cast< float >( x );
}

void MVector2::SetValues(float values[ 2 ])
{
	m_value.vec.x = values[ 0 ];
	m_value.vec.y = values[ 1 ];
}

void MVector2::SetValues(size_t values[ 2 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
}

void MVector2::SetValues(int32_t values[ 2 ])
{
	m_value.vec.x = static_cast< float >( values[ 0 ] );
	m_value.vec.y = static_cast< float >( values[ 1 ] );
}

float MVector2::GetX() const
{
	return m_value.vec.x;
}

float MVector2::GetY() const
{
	return m_value.vec.y;
}

const float* MVector2::GetValues() const
{
	return m_value.elements;
}

float MVector2::Length() const
{
	return std::sqrtf(m_value.vec.x * m_value.vec.x + m_value.vec.y * m_value.vec.y);
}

MVector2& MVector2::Normalization()
{
	float lLen = Length();

	if ( lLen != 0 )
	{
		return *this /= lLen;

	}

	return *this;
}

float MVector2::Dot(const MVector2& v_) const
{
	return m_value.vec.x * v_.m_value.vec.x + m_value.vec.y * v_.m_value.vec.y;
}

float MVector2::Cross(const MVector2& v_) const
{
	return m_value.vec.x * v_.m_value.vec.y - v_.m_value.vec.x * m_value.vec.y;
}

MVector2 MVector2::operator+() const
{
	return *this;
}

MVector2 MVector2::operator-() const
{
	return  MVector2(-m_value.vec.x,-m_value.vec.y);
}

MVector2& MVector2::operator+=(const MVector2& v_)
{
	m_value.vec.x += v_.m_value.vec.x;
	m_value.vec.y += v_.m_value.vec.y;

	return *this;
}

MVector2& MVector2::operator-=(const MVector2& v_)
{
	m_value.vec.x -= v_.m_value.vec.x;
	m_value.vec.y -= v_.m_value.vec.y;

	return *this;
}

MVector2& MVector2::operator*=(float s_)
{
	m_value.vec.x *= s_;
	m_value.vec.y *= s_;

	return *this;
}

MVector2& MVector2::operator/=(float s_)
{
	m_value.vec.x /= s_;
	m_value.vec.y /= s_;

	return *this;
}

MVector2& MVector2::operator/=(const MVector2& v_)
{
	m_value.vec.x /= v_.m_value.vec.x;
	m_value.vec.y /= v_.m_value.vec.y;
	return *this;
}

const MVector2 operator+(const MVector2& v1_,const MVector2& v2_)
{
	MVector2 lTmp(v1_);
	return lTmp += v2_;
}

const MVector2 operator-(const MVector2& v1_,const MVector2& v2_)
{
	MVector2 lTmp(v1_);
	return lTmp -= v2_;
}

const MVector2 operator*(const MVector2& v_,float s_)
{
	MVector2 lTmp(v_);
	return lTmp *= s_;
}

const MVector2 operator/(const MVector2& v_,float s_)
{
	MVector2 lTmp(v_);
	return lTmp /= s_;
}
