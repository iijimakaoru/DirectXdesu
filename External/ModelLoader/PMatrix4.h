#ifndef	__MATH_P_MATRIX4_H__
#define	__MATH_P_MATRIX4_H__

#include<P_Defined.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cstdint>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

class PMatrix4
{
public:

	PMatrix4();

	PMatrix4(float values[ 16 ]);

	PMatrix4(float m00,float m01,float m02,float m03,
			float m10,float m11,float m12,float m13,
			float m20,float m21,float m22,float m23,
			float m30,float m31,float m32,float m33);

	PMatrix4(const PMatrix4&);

public:

	float Get(size_t xIndex,size_t yIndex) const;
	const float* GetValues()const;

	void Set(size_t xIndex,size_t yIndex,float value);

	void SetMatrix(float m00,float m01,float m02,float m03,float m10,float m11,float m12,float m13,float m20,float m21,float m22,float m23,float m30,float m31,float m32,float m33);
	void SetValues(float values[ 16 ]);

	void MakeScaling(float x,float y,float z);

	void MakeTranslation(float x,float y,float z);

	void MakeRotationX(float angle);
	void MakeRotationY(float angle);
	void MakeRotationZ(float angle);

	void MakeRotation(float x,float y,float z);

	void MakeOrthographicMatrix(float left,float top,float right,float bottom,float near,float far);

	void MakeOrthogonalL(float width,float height,float near,float far);

	void MakePerspectiveL(float fovAngleY,float aspect,float near,float far);

	void MakePerspectiveFovMatrix(float fovAngleY,float aspect,float near,float far);

	void CoordinateTransformation3D(float& x,float& y,float& z) const;

	void MakeViewport(float width,float height);

	void Transpose();

	void MakeIdentity();

	void MakeInverse();

public:

	PMatrix4& operator=(const PMatrix4& mat);

	const PMatrix4& operator=(PMatrix4& mat);

	PMatrix4& operator+=(const PMatrix4& mat);

	PMatrix4& operator-=(const PMatrix4& mat);

	PMatrix4& operator*=(const PMatrix4& mat);

	PMatrix4 operator+(const PMatrix4& mat) const;

	PMatrix4 operator-(const PMatrix4& mat) const;

	PMatrix4 operator*(const PMatrix4& mat) const;

private:

	struct MAT
	{
		float mat00; float mat01; float mat02; float mat03;
		float mat10; float mat11; float mat12; float mat13;
		float mat20; float mat21; float mat22; float mat23;
		float mat30; float mat31; float mat32; float mat33;
	};

	union VALUE
	{
		float mat[ 4 ][ 4 ];

		float elements[ 16 ];

		MAT matrix;
	};

private:

	VALUE m_value;
};

PHONON_LOADER_NAMESPACE_END

#endif	// __MATH_P_MATRIX4_H__