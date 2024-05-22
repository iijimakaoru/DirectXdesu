#include "Matrix4.h"
#include "MyMath.h"
#include <cmath>
#include <stdexcept>

const float EPSILON = 0.000001f;

namespace KMyMath {
Matrix4::Matrix4() {
	m[0][0] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

Matrix4::Matrix4(
    float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

Vector3 Matrix4::transform(const Vector3& v, const Matrix4& m1) {
	float w = v.x * m1.m[0][3] + v.y * m1.m[1][3] + v.z * m1.m[2][3] + m1.m[3][3];

	Vector3 result{
	    (v.x * m1.m[0][0] + v.y * m1.m[1][0] + v.z * m1.m[2][0] + m1.m[3][0]) / w,
	    (v.x * m1.m[0][1] + v.y * m1.m[1][1] + v.z * m1.m[2][1] + m1.m[3][1]) / w,
	    (v.x * m1.m[0][2] + v.y * m1.m[1][2] + v.z * m1.m[2][2] + m1.m[3][2]) / w,
	};

	return result;
}

Matrix4& Matrix4::operator=(const Matrix4& m_) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = m_.m[i][j];
		}
	}

	return *this;
}

Matrix4& Matrix4::operator+=(const Matrix4& m_) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] += m_.m[i][j];
		}
	}

	return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& m_) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] -= m_.m[i][j];
		}
	}

	return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& m_) {
	Matrix4 temp(*this);

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			double f = 0.0;
			for (size_t k = 0; k < 4; k++) {
				f += (double)temp.m[i][k] * (double)m_.m[k][j];

				m[i][j] = (float)f;
			}
		}
	}
	return *this;
}

Matrix4 Matrix4::operator+(const Matrix4& m_) const {
	Matrix4 temp(*this);
	temp += m_;
	return temp;
}

Matrix4 Matrix4::operator-(const Matrix4& m_) const {
	Matrix4 temp(*this);
	temp -= m_;
	return temp;
}

Matrix4 Matrix4::operator*(const Matrix4& m_) const {
	Matrix4 temp(*this);
	temp *= m_;
	return temp;
}

// 2項演算子オーバーロード ( ベクトルと行列の積 )
const Vector3 operator*(const Vector3& v, const Matrix4& m2) {
	Matrix4 mat = MyMathUtility::MatUnit();
	Vector3 result = mat.transform(v, m2);
	return result;
}
} // namespace KMyMath