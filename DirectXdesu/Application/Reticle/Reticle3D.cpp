#include "Reticle3D.h"

void Reticle3D::Init()
{
	
}

void Reticle3D::Update(KMyMath::Matrix4& m_, KMyMath::Vector3 pPos_)
{
	// 自機から3Dへのレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;

	// 自機から3Dレティクルへのオフセット(Z + 向き)
	KMyMath::Vector3 offset = { 0,0,1.0f };

	// 自機からワールド行列の回転を反映
	offset = KMyMath::Vec3Mat4Mul(offset, m_);

	// ベクトルの長さを整える
	offset = MyMathUtility::MakeNormalize(offset) * kDistancePlayerTo3DReticle;

	// 3Dレティクルの座標設定
	transform.SetPos(pPos_ + offset);

	// 座標更新
	transform.TransUpdate();
}

KMyMath::Vector3 Reticle3D::GetWorldPos()
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = transform.GetWorldPos();

	return result;
}
