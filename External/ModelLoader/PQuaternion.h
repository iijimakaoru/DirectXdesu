#ifndef	__MATH_P_QUATERNION_H__
#define	__MATH_P_QUATERNION_H__

#include<P_Defined.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cstdint>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

class PQuaternion
{
public:
	PQuaternion(float x = 0.0f,float y = 0.0f,float z = 0.0f,float w = 1.0f);

	PQuaternion(const float vec[ 3 ],float angle);

	PQuaternion(float x,float y,float z);

	void SetEulerAngles(float x,float y,float z);

	void SetEulerAngles(const float vec[ 3 ]);

	void Set(float value);

	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetW(float w);

	void SetValue(float x,float y,float z,float w);

	void SetValues(float values[ 4 ]);

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;

	const float* GetValues() const;

	void GetEuler(float& roll,float& pitch,float& yaw ) const;

	float Dot(const PQuaternion& q)const;

	float Length()const;

	PQuaternion Normalization() const;
	PQuaternion& Normalization();

	PQuaternion Identity() const;

	PQuaternion Conjugate() const;

	PQuaternion Inverse() const;

	PQuaternion Slerp(const PQuaternion& q,float t);

	PQuaternion Nlerp(const PQuaternion& q,float t) const;

	PQuaternion Lerp(const PQuaternion& q,float t) const;

public:

	PQuaternion operator+();
	PQuaternion operator-()const;

	PQuaternion& operator+=(const PQuaternion& q);
	PQuaternion& operator-=(const PQuaternion& q);
	PQuaternion& operator*=(float s);
	PQuaternion& operator/=(float s);
	PQuaternion& operator*=(const PQuaternion& q);

	friend const PQuaternion operator+(const PQuaternion& q1,const PQuaternion& q2);
	friend const PQuaternion operator-(const PQuaternion& q1,const PQuaternion& q2);
	friend const PQuaternion operator*(const PQuaternion& q1,const PQuaternion& q2);
	friend const PQuaternion operator*(const PQuaternion& q,float s);
	friend const PQuaternion operator*(float s,const PQuaternion& q);
	friend const PQuaternion operator/(const PQuaternion& q,float s);

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

void QuaternionSlerp(PQuaternion& vOut,const PQuaternion& qStart,const PQuaternion& qEnd,float t);

PHONON_LOADER_NAMESPACE_END

#endif	// __MATH_P_QUATERNION_H__