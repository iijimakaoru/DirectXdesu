#include <PQuaternion.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cmath>
#include<algorithm>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

PQuaternion::PQuaternion(float x,float y,float z,float w)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
	m_value.vec.w = w;
}

PQuaternion::PQuaternion(const float vec[ 3 ],float angle)
{
	float sin = std::sinf(angle / 2.0f);

	m_value.vec.x = sin * vec[ 0 ];
	m_value.vec.y = sin * vec[ 1 ];
	m_value.vec.z = sin * vec[ 2 ];
	m_value.vec.w = std::cosf(angle / 2.0f);
}

PQuaternion::PQuaternion(float x,float y,float z)
{
	float cosX2 = std::cos(x * 0.5f);
	float cosY2 = std::cos(y * 0.5f);
	float cosZ2 = std::cos(z * 0.5f);

	float sinX2 = std::sin(x * 0.5f);
	float sinY2 = std::sin(y * 0.5f);
	float sinZ2 = std::sin(z * 0.5f);

	m_value.vec.w = cosZ2 * cosY2 * cosX2 + sinZ2 * sinY2 * sinX2;
	m_value.vec.x = cosZ2 * cosY2 * sinX2 - sinZ2 * sinY2 * cosX2;
	m_value.vec.y = cosZ2 * sinY2 * cosX2 + sinZ2 * cosY2 * sinX2;
	m_value.vec.z = sinZ2 * cosY2 * cosX2 - cosZ2 * sinY2 * sinX2;
}

void PQuaternion::SetEulerAngles(float x,float y,float z)
{
	float cosX2 = std::cos(x * 0.5f);
	float cosY2 = std::cos(y * 0.5f);
	float cosZ2 = std::cos(z * 0.5f);

	float sinX2 = std::sin(x * 0.5f);
	float sinY2 = std::sin(y * 0.5f);
	float sinZ2 = std::sin(z * 0.5f);

	m_value.vec.w = cosZ2 * cosY2 * cosX2 + sinZ2 * sinY2 * sinX2;
	m_value.vec.x = cosZ2 * cosY2 * sinX2 - sinZ2 * sinY2 * cosX2;
	m_value.vec.y = cosZ2 * sinY2 * cosX2 + sinZ2 * cosY2 * sinX2;
	m_value.vec.z = sinZ2 * cosY2 * cosX2 - cosZ2 * sinY2 * sinX2;
}

void PQuaternion::SetEulerAngles(const float vec[ 3 ])
{
	float cosX2 = std::cos(vec[0] * 0.5f);
	float cosY2 = std::cos(vec[1] * 0.5f);
	float cosZ2 = std::cos(vec[2] * 0.5f);

	float sinX2 = std::sin(vec[0] * 0.5f);
	float sinY2 = std::sin(vec[1] * 0.5f);
	float sinZ2 = std::sin(vec[2] * 0.5f);

	m_value.vec.w = cosZ2 * cosY2 * cosX2 + sinZ2 * sinY2 * sinX2;
	m_value.vec.x = cosZ2 * cosY2 * sinX2 - sinZ2 * sinY2 * cosX2;
	m_value.vec.y = cosZ2 * sinY2 * cosX2 + sinZ2 * cosY2 * sinX2;
	m_value.vec.z = sinZ2 * cosY2 * cosX2 - cosZ2 * sinY2 * sinX2;
}

void PQuaternion::Set(float value)
{
	m_value.vec.x = value;
	m_value.vec.y = value;
	m_value.vec.z = value;
	m_value.vec.w = value;
}

void PQuaternion::SetX(float x)
{
	m_value.vec.x = x;
}

void PQuaternion::SetY(float y)
{
	m_value.vec.y = y;
}

void PQuaternion::SetZ(float z)
{
	m_value.vec.z = z;
}

void PQuaternion::SetW(float w)
{
	m_value.vec.w = w;
}

void PQuaternion::SetValue(float x,float y,float z,float w)
{
	m_value.vec.x = x;
	m_value.vec.y = y;
	m_value.vec.z = z;
	m_value.vec.w = w;
}

void PQuaternion::SetValues(float values[ 4 ])
{
	for ( size_t i = 0; i < 4; i++ )
	{
		m_value.elements[i] = values[ i ];
	}
}

float PQuaternion::GetX() const
{
	return m_value.vec.x;
}

float PQuaternion::GetY() const
{
	return m_value.vec.y;
}

float PQuaternion::GetZ() const
{
	return m_value.vec.z;
}

float PQuaternion::GetW() const
{
	return m_value.vec.w;
}

const float* PQuaternion::GetValues() const
{
	return m_value.elements;
}

void PQuaternion::GetEuler(float& roll,float& pitch,float& yaw) const
{
	constexpr float P_PI = 3.1415926535897931f;

	float sinrCosp = 2.0f * ( m_value.vec.w * m_value.vec.x + m_value.vec.y * m_value.vec.z );
	float cosrCosp = 1.0f - 2.0f * ( m_value.vec.x * m_value.vec.x + m_value.vec.y * m_value.vec.y );
	roll = std::atan2(sinrCosp,cosrCosp);

	float sinp = 2.0f * ( m_value.vec.w * m_value.vec.y - m_value.vec.z * m_value.vec.x );

	if ( std::abs(sinp) >= 1.0 )
	{
		pitch = std::copysign(P_PI / 2,sinp);
	}
	else
	{
		pitch = std::asin(sinp);
	}

	float sinyCosp = 2.0f * ( m_value.vec.w * m_value.vec.z + m_value.vec.x * m_value.vec.y );
	float cosyCosp = 1.0f - 2.0f * ( m_value.vec.y * m_value.vec.y + m_value.vec.z * m_value.vec.z );
	yaw = std::atan2(sinyCosp,cosyCosp);
}

float PQuaternion::Dot(const PQuaternion& q) const
{
	return m_value.vec.x * q.m_value.vec.x + m_value.vec.y * q.m_value.vec.y + m_value.vec.z * q.m_value.vec.z + m_value.vec.w * q.m_value.vec.w;
}

float PQuaternion::Length() const
{
	return std::sqrtf(Dot(*this));
}

PQuaternion PQuaternion::Normalization() const
{
	PQuaternion ret(*this);

	float len = Length();

	if ( len != 0 )
	{
		return ret /= len;
	}

	return ret;
}

PQuaternion& PQuaternion::Normalization()
{
	float len = Length();

	if ( len != 0 )
	{
		return *this /= len;
	}

	return *this;
}

PQuaternion PQuaternion::Identity() const
{
	return PQuaternion();
}

PQuaternion PQuaternion::Conjugate() const
{
	return PQuaternion(-m_value.vec.x,-m_value.vec.y,-m_value.vec.z,m_value.vec.w);
}

PQuaternion PQuaternion::Inverse() const
{
	return Conjugate() / ( Length() * Length() );
}

PQuaternion PQuaternion::Slerp(const PQuaternion& q,float t)
{
	PQuaternion result;
	PQuaternion start = *this;
	PQuaternion end = q;

	float cosHalfTheta = start.Dot(end);

	if ( cosHalfTheta < 0.0f )
	{
		end.m_value.vec.x = -end.m_value.vec.x; end.m_value.vec.y = -end.m_value.vec.y; end.m_value.vec.z = -end.m_value.vec.z; end.m_value.vec.w = -end.m_value.vec.w;
		cosHalfTheta = -cosHalfTheta;
	}

	if ( std::abs(cosHalfTheta) >= 1.0f )
	{
		result = start;
	}
	else if ( cosHalfTheta > 0.95f )
	{
		result = start.Nlerp(end,t);
	}
	else
	{
		float halfTheta = std::acos(cosHalfTheta);
		float sinHalfTheta = std::sqrtf(1.0f - cosHalfTheta * cosHalfTheta);

		if ( std::abs(sinHalfTheta) < 0.001f )
		{
			result = ( start * 0.5f + end * 0.5f );
		}
		else
		{
			float satioA = std::sin(( 1 - t ) * halfTheta) / sinHalfTheta;
			float ratioB = std::sin(t * halfTheta) / sinHalfTheta;

			result = ( start * satioA + end * ratioB );
		}
	}

	return result;
}

PQuaternion PQuaternion::Nlerp(const PQuaternion& q,float t) const
{
	PQuaternion result;

	result = *this + t * ( q - *this );

	float length = result.Length();

	if ( length == 0.0f )
	{
		length = 1.0f;
	}

	float ilength = 1.0f / length;

	result = result * ilength;

	return result;
}

PQuaternion PQuaternion::Lerp(const PQuaternion& q,float t) const
{
	float dot = this->Dot(q);

	PQuaternion t2 = q;

	if ( dot < 0.0f )
	{
		dot = -dot;
		t2 = -q;
	}

	float k0 = 1.0f - t;
	float k1 = t;

	return *this * k0 + t2 * k1;
}

PQuaternion PQuaternion::operator+()
{
	return PQuaternion(std::abs(m_value.vec.x),std::abs(m_value.vec.y),std::abs(m_value.vec.z),std::abs(m_value.vec.w));
}

PQuaternion PQuaternion::operator-() const
{
	return PQuaternion(-m_value.vec.x,-m_value.vec.y,-m_value.vec.z,m_value.vec.w);
}

PQuaternion& PQuaternion::operator+=(const PQuaternion& q)
{
	m_value.vec.x += q.m_value.vec.x;
	m_value.vec.y += q.m_value.vec.y;
	m_value.vec.z += q.m_value.vec.z;
	m_value.vec.w += q.m_value.vec.w;

	return *this;
}

PQuaternion& PQuaternion::operator-=(const PQuaternion& q)
{
	m_value.vec.x -= q.m_value.vec.x;
	m_value.vec.y -= q.m_value.vec.y;
	m_value.vec.z -= q.m_value.vec.z;
	m_value.vec.w -= q.m_value.vec.w;

	return *this;
}

PQuaternion& PQuaternion::operator*=(float s)
{
	m_value.vec.x -= s;
	m_value.vec.y -= s;
	m_value.vec.z -= s;
	m_value.vec.w -= s;

	return *this;
}

PQuaternion& PQuaternion::operator/=(float s)
{
	return *this *= 1.0f / s;
}

PQuaternion& PQuaternion::operator*=(const PQuaternion& q)
{
	float rx,ry,rz,rw;

	rx = m_value.vec.w * q.m_value.vec.x + q.m_value.vec.w *   m_value.vec.x +  m_value.vec.y * q.m_value.vec.z + -m_value.vec.z * q.m_value.vec.y;
	ry = m_value.vec.w * q.m_value.vec.y + q.m_value.vec.w *   m_value.vec.y +  m_value.vec.z * q.m_value.vec.x + -m_value.vec.x * q.m_value.vec.z;
	rz = m_value.vec.w * q.m_value.vec.z + q.m_value.vec.w *   m_value.vec.z +  m_value.vec.x * q.m_value.vec.y + -m_value.vec.y * q.m_value.vec.x;
	rw = m_value.vec.w * q.m_value.vec.w +  -m_value.vec.x * q.m_value.vec.x + -m_value.vec.y * q.m_value.vec.y + -m_value.vec.z * q.m_value.vec.z;

	m_value.vec.x = rx;
	m_value.vec.y = ry;
	m_value.vec.z = rz;
	m_value.vec.w = rw;

	return *this;
}

const PQuaternion operator+(const PQuaternion& q1,const PQuaternion& q2)
{
	PQuaternion ret = q1;

	return ret += q2;
}

const PQuaternion operator-(const PQuaternion& q1,const PQuaternion& q2)
{
	PQuaternion ret = q1;
	return ret -= q2;
}

const PQuaternion operator*(const PQuaternion& q1,const PQuaternion& q2)
{
	PQuaternion ret = q1;

	return ret *= q2;
}

const PQuaternion operator*(const PQuaternion& q,float s)
{
	PQuaternion ret = q;

	return ret *= s;
}

const PQuaternion operator*(float s,const PQuaternion& q)
{
	PQuaternion ret = q;

	return ret *= s;
}

const PQuaternion operator/(const PQuaternion& q,float s)
{
	PQuaternion ret = q;
	ret /= s;
	return ret;
}

void QuaternionSlerp(PQuaternion& vOut,const PQuaternion& qStart,const PQuaternion& qEnd,float t)
{
	PQuaternion start = PQuaternion(qStart);
	PQuaternion end = PQuaternion(qEnd);

	vOut = start.Slerp(end,t);
}

PHONON_LOADER_NAMESPACE_END