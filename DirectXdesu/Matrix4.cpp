#include "Matrix4.h"

Matrix4::Matrix4() {

}

Matrix4::Matrix4(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33) {

}

Matrix4 Matrix4::Identity() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				mat[i][j] = 1;
			}
			else {
				mat[i][j] = 0;
			}
		}
	}
	return *this;
}

Matrix4 Matrix4::AllzeroInit() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	return *this;
}