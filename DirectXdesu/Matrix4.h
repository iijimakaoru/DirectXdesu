#pragma once
class Matrix4
{
public:
	float mat[4][4];

	// コンストラクタ
	Matrix4();
	// 成分を入れての宣言
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	// 単位行列
	Matrix4 Identity();
	// 全部0にする関数
	Matrix4 AllzeroInit();

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);
};

