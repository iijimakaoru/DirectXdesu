#ifndef	__CAPTURE_VECTOR5_H__
#define	__CAPTURE_VECTOR5_H__

#include<cstdint>

class CaptureVector5
{
public:
	CaptureVector5();
	CaptureVector5(float x,float y,float z,float w,float v);
	CaptureVector5(int32_t x,int32_t y,int32_t z,int32_t w,int32_t v);
	CaptureVector5(size_t x,size_t y,size_t z,size_t w,size_t v);
	CaptureVector5(float values[ 5 ]);

public:

	void Set(float value);

	void Set(size_t index, float value);

	void SetX(float x);
	void SetX(size_t x);
	void SetX(int32_t x);

	void SetY(float y);
	void SetY(size_t y);
	void SetY(int32_t y);

	void SetZ(float z);
	void SetZ(size_t z);
	void SetZ(int32_t z);

	void SetW(float w);
	void SetW(size_t w);
	void SetW(int32_t w);

	void SetV(float v);
	void SetV(size_t v);
	void SetV(int32_t v);

	void SetValue(float x,float y,float z,float w,float v);
	void SetValue(int32_t x,int32_t y,int32_t z,int32_t w,int32_t v);
	void SetValue(size_t x,size_t y,size_t z,size_t w,size_t v);

	void SetValues(float values[ 5 ]);
	void SetValues(size_t values[ 5 ]);
	void SetValues(int32_t values[ 5 ]);

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;
	float GetV() const;

	const float* GetValues() const;

	float Length()const;
	CaptureVector5 Normalization()const;
	CaptureVector5& Normalization();
	float Dot(const CaptureVector5& v)const;
	CaptureVector5 Cross(const CaptureVector5& v1,const CaptureVector5& v2)const;

public:

	CaptureVector5 operator+() const;
	CaptureVector5 operator-() const;

	CaptureVector5& operator+=(const CaptureVector5& v);
	CaptureVector5& operator-=(const CaptureVector5& v);
	CaptureVector5& operator*=(float s);
	CaptureVector5& operator/=(float s);

	friend const CaptureVector5 operator+(const CaptureVector5& v1,const CaptureVector5& v2);
	friend const CaptureVector5 operator-(const CaptureVector5& v1,const CaptureVector5& v2);
	friend const CaptureVector5 operator*(const CaptureVector5& v,float s);
	friend const CaptureVector5 operator*(float s,const CaptureVector5& v);
	friend const CaptureVector5 operator/(const CaptureVector5& v,float s);

private:

	struct VEC
	{
		float x;
		float y;
		float z;
		float w;
		float v;
	};

	union VALUE
	{
		float elements[ 5 ];

		VEC vec;
	};

private:

	VALUE m_value;
};

#endif	// __CAPTURE_VECTOR4_H__

