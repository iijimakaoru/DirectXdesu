#include <CaptureMatrix3.h>

#include<cmath>
#include<cassert>

CaptureMatrix3::CaptureMatrix3()
{
	m_value.mat[ 0 ][ 0 ] = 1.0f;
	m_value.mat[ 0 ][ 1 ] = 0.0f;
	m_value.mat[ 0 ][ 2 ] = 0.0f;

	m_value.mat[ 1 ][ 0 ] = 0.0f;
	m_value.mat[ 1 ][ 1 ] = 1.0f;
	m_value.mat[ 1 ][ 2 ] = 0.0f;

	m_value.mat[ 2 ][ 0 ] = 0.0f;
	m_value.mat[ 2 ][ 1 ] = 0.0f;
	m_value.mat[ 2 ][ 2 ] = 1.0f;
}

CaptureMatrix3::CaptureMatrix3(float m00,float m01,float m02,float m10,float m11,float m12,float m20,float m21,float m22)
{
	m_value.mat[ 0 ][ 0 ] = m00;
	m_value.mat[ 0 ][ 1 ] = m01;
	m_value.mat[ 0 ][ 2 ] = m02;

	m_value.mat[ 1 ][ 0 ] = m10;
	m_value.mat[ 1 ][ 1 ] = m11;
	m_value.mat[ 1 ][ 2 ] = m12;

	m_value.mat[ 2 ][ 0 ] = m20;
	m_value.mat[ 2 ][ 1 ] = m21;
	m_value.mat[ 2 ][ 2 ] = m22;
}

CaptureMatrix3 CaptureMatrix3::MakeMat3Identity()
{
	CaptureMatrix3 mat;
	return mat;
}

CaptureMatrix3 CaptureMatrix3::MakeInverse()
{
	float sweep[ 3 ][ 6 ]{};
	float constTimes = 0.0f;
	const float MAX_ERR = 1e-10f;
	CaptureMatrix3 retMat;

	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = 0; j < 3; j++ )
		{
			sweep[ i ][ j ] = m_value.mat[ i ][ j ];

			sweep[ i ][ 3 + j ] = MakeMat3Identity().m_value.mat[ i ][ j ];
		}
	}

	for ( size_t i = 0; i < 3; i++ )
	{
		float lMax = std::fabs(sweep[ i ][ i ]);
		size_t lMaxIndex = i;

		for ( size_t j = i + 1; j < 3; j++ )
		{
			if ( std::fabs(sweep[ j ][ i ]) > lMax )
			{
				lMax = std::fabs(sweep[ j ][ i ]);
				lMaxIndex = j;
			}
		}

		if ( fabs(sweep[ lMaxIndex ][ i ]) <= MAX_ERR )
		{
			return MakeMat3Identity();
		}

		if ( i != lMaxIndex )
		{
			for ( size_t j = 0; j < 6; j++ )
			{
				float tmp = sweep[ lMaxIndex ][ j ];
				sweep[ lMaxIndex ][ j ] = sweep[ i ][ j ];
				sweep[ i ][ j ] = tmp;
			}
		}

		constTimes = 1 / sweep[ i ][ i ];

		for ( size_t j = 0; j < 6; j++ )
		{
			sweep[ i ][ j ] *= constTimes;
		}

		for ( size_t j = 0; j < 6; j++ )
		{
			if ( j == i )
			{
				continue;
			}

			constTimes = -sweep[ j ][ i ];

			for ( size_t k = 0; k < 6; k++ )
			{
				sweep[ j ][ k ] += sweep[ i ][ k ] * constTimes;
			}
		}
	}

	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = 0; j < 3; j++ )
		{
			retMat.m_value.mat[ i ][ j ] = sweep[ i ][ 3 + j ];
		}
	}

	return retMat;
}

void CaptureMatrix3::MakeScaling(float x,float y)
{
	m_value.mat[ 0 ][ 0 ] = x;
	m_value.mat[ 1 ][ 1 ] = y;
}

void CaptureMatrix3::MakeTranslation(float x,float y)
{
	m_value.mat[ 3 ][ 0 ] = x;
	m_value.mat[ 3 ][ 1 ] = y;
}

void CaptureMatrix3::MakeRotation(float angle)
{
	m_value.mat[ 0 ][ 0 ] = std::cosf(angle);
	m_value.mat[ 0 ][ 1 ] = -std::sinf(angle);

	m_value.mat[ 1 ][ 0 ] = std::sinf(angle);
	m_value.mat[ 1 ][ 1 ] = std::cosf(angle);
}

CaptureMatrix3 CaptureMatrix3::Transpose()
{
	CaptureMatrix3 tmp(*this);
	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = i; j < 3; j++ )
		{
			float lF = tmp.m_value.mat[ i ][ j ];
			tmp.m_value.mat[ i ][ j ] = tmp.m_value.mat[ j ][ i ];
			tmp.m_value.mat[ j ][ i ] = lF;
		}
	}

	return tmp;
}

float CaptureMatrix3::Get(size_t xIndex,size_t yIndex) const
{
	return m_value.mat[ yIndex ][ xIndex ];
}

const float* CaptureMatrix3::GetValues() const
{
	return m_value.elements;
}

void CaptureMatrix3::Set(size_t xIndex,size_t yIndex,float value)
{
	m_value.mat[ yIndex ][ xIndex ] = value;
}

void CaptureMatrix3::SetMatrix(float m00,float m01,float m02,float m10,float m11,float m12,float m20,float m21,float m22)
{
	m_value.mat[ 0 ][ 0 ] = m00;
	m_value.mat[ 0 ][ 1 ] = m01;
	m_value.mat[ 0 ][ 2 ] = m02;

	m_value.mat[ 1 ][ 0 ] = m10;
	m_value.mat[ 1 ][ 1 ] = m11;
	m_value.mat[ 1 ][ 2 ] = m12;

	m_value.mat[ 2 ][ 0 ] = m20;
	m_value.mat[ 2 ][ 1 ] = m21;
	m_value.mat[ 2 ][ 2 ] = m22;
}

void CaptureMatrix3::SetValues(float values[ 9 ])
{
	for ( size_t i = 0; i < 9; i++ )
	{
		m_value.elements[ i ] = values[ i ];
	}
}

CaptureMatrix3::CaptureMatrix3(const CaptureMatrix3&) = default;

CaptureMatrix3& CaptureMatrix3::operator=(const CaptureMatrix3& mat)
{
	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = 0; j < 3; j++ )
		{
			m_value.mat[ i ][ j ] = mat.m_value.mat[ i ][ j ];
		}
	}

	return *this;
}

const CaptureMatrix3& CaptureMatrix3::operator=(CaptureMatrix3& mat)
{
	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = 0; j < 3; j++ )
		{
			m_value.mat[ i ][ j ] = mat.m_value.mat[ i ][ j ];
		}
	}

	return *this;
}

CaptureMatrix3& CaptureMatrix3::operator+=(const CaptureMatrix3& mat)
{
	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = 0; j < 3; j++ )
		{
			m_value.mat[ i ][ j ] += mat.m_value.mat[ i ][ j ];
		}
	}

	return *this;
}

CaptureMatrix3& CaptureMatrix3::operator-=(const CaptureMatrix3& mat)
{
	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = 0; j < 3; j++ )
		{
			m_value.mat[ i ][ j ] -= mat.m_value.mat[ i ][ j ];
		}
	}

	return *this;
}

CaptureMatrix3& CaptureMatrix3::operator*=(const CaptureMatrix3& mat)
{
	CaptureMatrix3 tmp(*this);

	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = 0; j < 3; j++ )
		{
			double lF = 0.0;

			for ( size_t k = 0; k < 3; k++ )
			{
				lF += static_cast< double >( tmp.m_value.mat[ i ][ k ] ) * static_cast< double >( mat.m_value.mat[ k ][ j ] );

				m_value.mat[ i ][ j ] = static_cast< float >( lF );
			}
		}
	}
	return *this;
}

CaptureMatrix3 CaptureMatrix3::operator+(const CaptureMatrix3& mat) const
{
	CaptureMatrix3 tmp(*this);
	tmp += mat;
	return tmp;
}

CaptureMatrix3 CaptureMatrix3::operator-(const CaptureMatrix3& mat) const
{
	CaptureMatrix3 tmp(*this);
	tmp -= mat;
	return tmp;
}

CaptureMatrix3 CaptureMatrix3::operator*(const CaptureMatrix3& mat) const
{
	CaptureMatrix3 tmp(*this);
	tmp *= mat;
	return tmp;
}
