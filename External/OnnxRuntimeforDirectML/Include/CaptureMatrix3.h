#ifndef	__CAPTURE_MATRIX3_H__
#define	__CAPTURE_MATRIX3_H__

#include<cstdint>

struct  CaptureMatrix3
{
public:

	CaptureMatrix3();

	CaptureMatrix3(float m00,float m01,float m02,float m10,float m11,float m12,float m20,float m21,float m22);

	CaptureMatrix3(const CaptureMatrix3&);

public:

	void MakeScaling(float x,float y);

	void MakeTranslation(float x,float y);

	void MakeRotation(float angle);

	CaptureMatrix3 MakeMat3Identity();

	CaptureMatrix3 MakeInverse();

	CaptureMatrix3 Transpose();

	float Get(size_t xIndex,size_t yIndex) const;
	const float* GetValues()const;

	void Set(size_t xIndex,size_t yIndex,float value);

	void SetMatrix(float m00,float m01,float m02,float m10,float m11,float m12,float m20,float m21,float m22);
	void SetValues(float values[ 9 ]);

public:

	CaptureMatrix3& operator=(const CaptureMatrix3& mat);

	const CaptureMatrix3& operator=(CaptureMatrix3& mat);

	CaptureMatrix3& operator+=(const CaptureMatrix3& mat);

	CaptureMatrix3& operator-=(const CaptureMatrix3& mat);

	CaptureMatrix3& operator*=(const CaptureMatrix3& mat);

	CaptureMatrix3 operator+(const CaptureMatrix3& mat) const;

	CaptureMatrix3 operator-(const CaptureMatrix3& mat) const;

	CaptureMatrix3 operator*(const CaptureMatrix3& mat) const;

private:

	struct MAT
	{
		float m_mat00; float m_mat01; float m_mat02;
		float m_mat10; float m_mat11; float m_mat12;
		float m_mat20; float m_mat21; float m_mat22;
	};

	union VALUE
	{
		float mat[ 3 ][ 3 ];

		float elements[ 9 ];

		MAT matrix;
	};

private:

	VALUE m_value;
};

#endif	// __CAPTURE_MATRIX3_H__

