#include<PMatrix4.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cmath>
#include<algorithm>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

PMatrix4::PMatrix4()
{
	m_value.mat[ 0 ][ 0 ] = 1.0f;
	m_value.mat[ 0 ][ 1 ] = 0.0f;
	m_value.mat[ 0 ][ 2 ] = 0.0f;
	m_value.mat[ 0 ][ 3 ] = 0.0f;

	m_value.mat[ 1 ][ 0 ] = 0.0f;
	m_value.mat[ 1 ][ 1 ] = 1.0f;
	m_value.mat[ 1 ][ 2 ] = 0.0f;
	m_value.mat[ 1 ][ 3 ] = 0.0f;

	m_value.mat[ 2 ][ 0 ] = 0.0f;
	m_value.mat[ 2 ][ 1 ] = 0.0f;
	m_value.mat[ 2 ][ 2 ] = 1.0f;
	m_value.mat[ 2 ][ 3 ] = 0.0f;

	m_value.mat[ 3 ][ 0 ] = 0.0f;
	m_value.mat[ 3 ][ 1 ] = 0.0f;
	m_value.mat[ 3 ][ 2 ] = 0.0f;
	m_value.mat[ 3 ][ 3 ] = 1.0f;
}

PMatrix4::PMatrix4(float values[ 16 ])
{
	for ( size_t i = 0; i < 16; i++ )
	{
		m_value.elements[ i ] = values[ i ];
	}
}

PMatrix4::PMatrix4(float m00,float m01,float m02,float m03,float m10,float m11,float m12,float m13,float m20,float m21,float m22,float m23,float m30,float m31,float m32,float m33)
{
	m_value.mat[ 0 ][ 0 ] = m00;
	m_value.mat[ 0 ][ 1 ] = m01;
	m_value.mat[ 0 ][ 2 ] = m02;
	m_value.mat[ 0 ][ 3 ] = m03;

	m_value.mat[ 1 ][ 0 ] = m10;
	m_value.mat[ 1 ][ 1 ] = m11;
	m_value.mat[ 1 ][ 2 ] = m12;
	m_value.mat[ 1 ][ 3 ] = m13;

	m_value.mat[ 2 ][ 0 ] = m20;
	m_value.mat[ 2 ][ 1 ] = m21;
	m_value.mat[ 2 ][ 2 ] = m22;
	m_value.mat[ 2 ][ 3 ] = m23;

	m_value.mat[ 3 ][ 0 ] = m30;
	m_value.mat[ 3 ][ 1 ] = m31;
	m_value.mat[ 3 ][ 2 ] = m32;
	m_value.mat[ 3 ][ 3 ] = m33;
}

PMatrix4::PMatrix4(const PMatrix4&) = default;


float PMatrix4::Get(size_t xIndex,size_t yIndex) const
{
	return m_value.mat[ yIndex ][ xIndex ];
}

const float* PMatrix4::GetValues() const
{
	return m_value.elements;
}

void PMatrix4::Set(size_t xIndex,size_t yIndex,float value)
{
	m_value.mat[ yIndex ][ xIndex ] = value;
}

void PMatrix4::SetMatrix(float m00,float m01,float m02,float m03,float m10,float m11,float m12,float m13,float m20,float m21,float m22,float m23,float m30,float m31,float m32,float m33)
{
	m_value.mat[ 0 ][ 0 ] = m00;
	m_value.mat[ 0 ][ 1 ] = m01;
	m_value.mat[ 0 ][ 2 ] = m02;
	m_value.mat[ 0 ][ 3 ] = m03;

	m_value.mat[ 1 ][ 0 ] = m10;
	m_value.mat[ 1 ][ 1 ] = m11;
	m_value.mat[ 1 ][ 2 ] = m12;
	m_value.mat[ 1 ][ 3 ] = m13;

	m_value.mat[ 2 ][ 0 ] = m20;
	m_value.mat[ 2 ][ 1 ] = m21;
	m_value.mat[ 2 ][ 2 ] = m22;
	m_value.mat[ 2 ][ 3 ] = m23;

	m_value.mat[ 3 ][ 0 ] = m30;
	m_value.mat[ 3 ][ 1 ] = m31;
	m_value.mat[ 3 ][ 2 ] = m32;
	m_value.mat[ 3 ][ 3 ] = m33;
}

void PMatrix4::SetValues(float values[ 16 ])
{
	for ( size_t i = 0; i < 16; i++ )
	{
		m_value.elements[ i ] = values[ i ];
	}
}

void PMatrix4::MakeScaling(float x,float y,float z)
{
	m_value.mat[ 0 ][ 0 ] = x;
	m_value.mat[ 1 ][ 1 ] = y;
	m_value.mat[ 2 ][ 2 ] = z;
}

void PMatrix4::MakeTranslation(float x,float y,float z)
{
	m_value.mat[ 3 ][ 0 ] = x;
	m_value.mat[ 3 ][ 1 ] = y;
	m_value.mat[ 3 ][ 2 ] = z;
}

void PMatrix4::MakeRotationX(float angle)
{
	m_value.mat[ 1 ][ 1 ] = std::cos(angle);
	m_value.mat[ 1 ][ 2 ] = std::sin(angle);

	m_value.mat[ 2 ][ 1 ] = -std::sin(angle);
	m_value.mat[ 2 ][ 2 ] = std::cos(angle);
}

void PMatrix4::MakeRotationY(float angle)
{
	m_value.mat[ 0 ][ 0 ] = std::cos(angle);
	m_value.mat[ 0 ][ 2 ] = -std::sin(angle);

	m_value.mat[ 2 ][ 0 ] = std::sin(angle);
	m_value.mat[ 2 ][ 2 ] = std::cos(angle);
}

void PMatrix4::MakeRotationZ(float angle)
{
	m_value.mat[ 0 ][ 0 ] = std::cos(angle);
	m_value.mat[ 0 ][ 1 ] = std::sin(angle);

	m_value.mat[ 1 ][ 0 ] = -std::sin(angle);
	m_value.mat[ 1 ][ 1 ] = std::cos(angle);
}

void PMatrix4::MakeRotation(float x,float y,float z)
{

	PMatrix4 rotX;
	PMatrix4 rotY;
	PMatrix4 rotZ;

	rotX.MakeRotationX(x);
	rotY.MakeRotationY(y);
	rotZ.MakeRotationZ(z);

	*this = rotZ * rotX * rotY;
}

void PMatrix4::MakeOrthographicMatrix(float left,float top,float right,float bottom,float near,float far)
{
	m_value.mat[ 0 ][ 0 ] = 2.0f / ( right - left );
	m_value.mat[ 1 ][ 1 ] = 2.0f / ( top - bottom );
	m_value.mat[ 2 ][ 2 ] = 1.0f / ( far - near );
	m_value.mat[ 3 ][ 0 ] = ( left + right ) / ( left - right );
	m_value.mat[ 3 ][ 1 ] = ( top + bottom ) / ( bottom - top );
	m_value.mat[ 3 ][ 2 ] = near / ( near - far );
	m_value.mat[ 3 ][ 3 ] = 1.0f;
}

void PMatrix4::MakeOrthogonalL(float width,float height,float near,float far)
{
	float fRange = 1.0f / ( far - near );

	m_value.mat[ 0 ][ 0 ] = 2.0f / width;

	m_value.mat[ 1 ][ 1 ] = 2.0f / height;

	m_value.mat[ 2 ][ 2 ] = fRange;

	m_value.mat[ 3 ][ 2 ] = -fRange * near;
	m_value.mat[ 3 ][ 3 ] = 1.0f;

	m_value.mat[ 0 ][ 1 ] = m_value.mat[ 0 ][ 2 ] = m_value.mat[ 0 ][ 3 ] = 0.0f;
	m_value.mat[ 1 ][ 0 ] = m_value.mat[ 1 ][ 2 ] = m_value.mat[ 1 ][ 3 ] = 0.0f;
	m_value.mat[ 2 ][ 0 ] = m_value.mat[ 2 ][ 1 ] = m_value.mat[ 2 ][ 3 ] = 0.0f;
	m_value.mat[ 3 ][ 0 ] = m_value.mat[ 3 ][ 1 ] = 0.0f;
}

void PMatrix4::MakePerspectiveL(float fovAngleY,float aspect,float near,float far)
{
	float sinFov = std::sinf(0.5f * fovAngleY);
	float cosFov = std::cosf(0.5f * fovAngleY);;

	float range = far / ( far - near );
	float height = cosFov / sinFov;

	m_value.mat[ 0 ][ 0 ] = height / aspect;

	m_value.mat[ 1 ][ 1 ] = cosFov / sinFov;

	m_value.mat[ 2 ][ 2 ] = range;
	m_value.mat[ 2 ][ 3 ] = 1.0f;

	m_value.mat[ 3 ][ 2 ] = -range * near;

	m_value.mat[ 0 ][ 1 ] = m_value.mat[ 0 ][ 2 ] = m_value.mat[ 0 ][ 3 ] = 0.0f;
	m_value.mat[ 1 ][ 0 ] = m_value.mat[ 1 ][ 2 ] = m_value.mat[ 1 ][ 3 ] = 0.0f;
	m_value.mat[ 2 ][ 0 ] = m_value.mat[ 2 ][ 1 ] = 0.0f;
	m_value.mat[ 3 ][ 0 ] = m_value.mat[ 3 ][ 1 ] = m_value.mat[ 3 ][ 3 ] = 0.0f;
}

void PMatrix4::MakePerspectiveFovMatrix(float fovAngleY,float aspect,float near,float far)
{
	m_value.mat[ 0 ][ 0 ] = ( 1 / aspect ) * ( 1 / std::tan(fovAngleY / 2) );
	m_value.mat[ 1 ][ 1 ] = 1 / std::tan(fovAngleY / 2);
	m_value.mat[ 2 ][ 2 ] = far / ( far - near );
	m_value.mat[ 2 ][ 3 ] = 1;
	m_value.mat[ 3 ][ 2 ] = -( near * far ) / ( fovAngleY - near );
}

void PMatrix4::CoordinateTransformation3D(float& x,float& y,float& z) const
{
	float retX = x;
	float retY = y;
	float retZ = z;

	retX = x * m_value.mat[ 0 ][ 0 ] + y * m_value.mat[ 1 ][ 0 ] + z * m_value.mat[ 2 ][ 0 ];
	retY = x * m_value.mat[ 0 ][ 1 ] + y * m_value.mat[ 1 ][ 1 ] + z * m_value.mat[ 2 ][ 1 ];
	retZ = x * m_value.mat[ 0 ][ 2 ] + y * m_value.mat[ 1 ][ 2 ] + z * m_value.mat[ 2 ][ 2 ];

	x = retX;
	y = retY;
	z = retZ;
}

void PMatrix4::MakeViewport(float width,float height)
{
	float winH = height / 2.0f;
	float winW = width / 2.0f;

	*this = { winW, 0, 0, 0, 0, -winH, 0, 0, 0, 0, 1, 0, winW, winH, 0, 1 };
}

void PMatrix4::Transpose()
{
	for ( size_t i = 0; i < 4; i++ )
	{
		for ( size_t j = i; j < 4; j++ )
		{
			float f = m_value.mat[ i ][ j ];

			m_value.mat[ i ][ j ] = m_value.mat[ j ][ i ];
			m_value.mat[ j ][ i ] = f;
		}
	}
}

void PMatrix4::MakeIdentity()
{
	*this = PMatrix4();
}

void PMatrix4::MakeInverse()
{
	float sweep[ 4 ][ 8 ]{};
	float constTimes = 0.0f;
	const float MAX_ERR = 1e-10f;
	PMatrix4 identityMat;

	for ( size_t i = 0; i < 4; i++ )
	{
		for ( size_t j = 0; j < 4; j++ )
		{
			sweep[ i ][ j ] = m_value.mat[ i ][ j ];

			sweep[ i ][ 4 + j ] = identityMat.m_value.mat[ i ][ j ];
		}
	}

	for ( size_t i = 0; i < 4; i++ )
	{
		float max = std::fabs(sweep[ i ][ i ]);
		size_t maxIndex = i;

		for ( size_t j = i + 1; j < 4; j++ )
		{
			if ( std::fabs(sweep[ j ][ i ]) > max )
			{
				max = std::fabs(sweep[ j ][ i ]);
				maxIndex = j;
			}
		}

		if ( std::fabs(sweep[ maxIndex ][ i ]) <= MAX_ERR )
		{
			*this = identityMat;
		}

		if ( i != maxIndex )
		{
			for ( size_t j = 0; j < 8; j++ )
			{
				float lTmp = sweep[ maxIndex ][ j ];
				sweep[ maxIndex ][ j ] = sweep[ i ][ j ];
				sweep[ i ][ j ] = lTmp;
			}
		}

		constTimes = 1 / sweep[ i ][ i ];

		for ( size_t j = 0; j < 8; j++ )
		{
			sweep[ i ][ j ] *= constTimes;
		}

		for ( size_t j = 0; j < 4; j++ )
		{
			if ( j == i )
			{
				continue;
			}

			constTimes = -sweep[ j ][ i ];

			for ( size_t k = 0; k < 8; k++ )
			{
				sweep[ j ][ k ] += sweep[ i ][ k ] * constTimes;
			}
		}
	}

	for ( size_t i = 0; i < 4; i++ )
	{
		for ( size_t j = 0; j < 4; j++ )
		{
			m_value.mat[ i ][ j ] = sweep[ i ][ 4 + j ];
		}
	}
}

PMatrix4& PMatrix4::operator=(const PMatrix4& mat)
{
	for ( size_t i = 0; i < 4; i++ )
	{
		for ( size_t j = 0; j < 4; j++ )
		{
			m_value.mat[ i ][ j ] = mat.m_value.mat[ i ][ j ];
		}
	}

	return *this;
}

const PMatrix4& PMatrix4::operator=(PMatrix4& mat)
{
	for ( size_t i = 0; i < 4; i++ )
	{
		for ( size_t j = 0; j < 4; j++ )
		{
			m_value.mat[ i ][ j ] = mat.m_value.mat[ i ][ j ];
		}
	}

	return *this;
}

PMatrix4& PMatrix4::operator+=(const PMatrix4& mat)
{
	for ( size_t i = 0; i < 4; i++ )
	{
		for ( size_t j = 0; j < 4; j++ )
		{
			m_value.mat[ i ][ j ] += mat.m_value.mat[ i ][ j ];
		}
	}

	return *this;
}

PMatrix4& PMatrix4::operator-=(const PMatrix4& mat)
{
	for ( size_t i = 0; i < 4; i++ )
	{
		for ( size_t j = 0; j < 4; j++ )
		{
			m_value.mat[ i ][ j ] -= mat.m_value.mat[ i ][ j ];
		}
	}

	return *this;
}

PMatrix4& PMatrix4::operator*=(const PMatrix4& mat)
{
	PMatrix4 tmp(*this);

	for ( size_t i = 0; i < 4; i++ )
	{
		for ( size_t j = 0; j < 4; j++ )
		{
			double lF = 0.0;

			for ( size_t k = 0; k < 4; k++ )
			{
				lF += static_cast< double >( tmp.m_value.mat[ i ][ k ] ) * static_cast< double >( mat.m_value.mat[ k ][ j ] );

				m_value.mat[ i ][ j ] = static_cast< float >( lF );
			}
		}
	}

	return *this;
}

PMatrix4 PMatrix4::operator+(const PMatrix4& mat) const
{
	PMatrix4 tmp(*this);
	tmp += mat;
	return tmp;
}

PMatrix4 PMatrix4::operator-(const PMatrix4& mat) const
{
	PMatrix4 tmp(*this);
	tmp -= mat;
	return tmp;
}

PMatrix4 PMatrix4::operator*(const PMatrix4& mat) const
{
	PMatrix4 tmp(*this);
	tmp *= mat;
	return tmp;
}

PHONON_LOADER_NAMESPACE_END