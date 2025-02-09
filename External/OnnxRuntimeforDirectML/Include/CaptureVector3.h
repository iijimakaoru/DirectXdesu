#ifndef	__CAPTURE_VECTOR3_H__
#define	__CAPTURE_VECTOR3_H__

#include<cstdint>

class CaptureVector3
{
public:

	CaptureVector3();
	CaptureVector3(float x,float y,float z);
	CaptureVector3(int32_t x,int32_t y,int32_t z);
	CaptureVector3(size_t x,size_t y,size_t z);
	CaptureVector3(float values[ 3 ]);

public:

	void Set(size_t index,float val);


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
	void SetValue(size_t x,size_t y,size_t z);
	void SetValue(int32_t x,int32_t y,int32_t z);

	void SetValues(float values[ 3 ]);
	void SetValues(size_t values[ 3 ]);
	void SetValues(int32_t values[ 3 ]);

	float GetX() const;
	float GetY() const;
	float GetZ() const;

	const float* GetValues() const;

	float Length()const;
	CaptureVector3 Normalization()const;
	CaptureVector3& Normalization();
	float Dot(const CaptureVector3& v)const;
	CaptureVector3 Cross(const CaptureVector3& v)const;
	CaptureVector3 Negate() const;

public:

	CaptureVector3 operator+() const;
	CaptureVector3 operator-() const;

	CaptureVector3& operator+=(const CaptureVector3& v);
	CaptureVector3& operator-=(const CaptureVector3& v);
	CaptureVector3& operator*=(const CaptureVector3& v);
	CaptureVector3& operator-=(float s);
	CaptureVector3& operator*=(float s);
	CaptureVector3& operator/=(float s);

	friend const CaptureVector3 operator+(const CaptureVector3& v1,const CaptureVector3& v2);
	friend const CaptureVector3 operator-(const CaptureVector3& v1,const CaptureVector3& v2);
	friend const CaptureVector3 operator-(const CaptureVector3& v,float s);
	friend const CaptureVector3 operator*(const CaptureVector3& v,float s);
	friend const CaptureVector3 operator*(const CaptureVector3& v,const CaptureVector3& v2);
	friend const CaptureVector3 operator*(float s,const CaptureVector3& v);
	friend const CaptureVector3 operator/(const CaptureVector3& v,float s);

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

#endif	// __CAPTURE_VECTOR3_H__