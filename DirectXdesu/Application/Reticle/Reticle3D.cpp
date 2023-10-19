#include "Reticle3D.h"

void Reticle3D::Init()
{
	// オブジェクト生成
	object = std::make_unique<KObject3d>();
}

void Reticle3D::Update(KMyMath::Matrix4& m, KMyMath::Vector3 pPos)
{
	// 自機から3Dへのレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;

	// 自機から3Dレティクルへのオフセット(Z + 向き)
	KMyMath::Vector3 offset = { 0,0,1.0f };

	// 自機からワールド行列の回転を反映
	offset = KMyMath::Vec3Mat4Mul(offset, m);

	// ベクトルの長さを整える
	offset = MyMathUtility::MakeNormalize(offset) * kDistancePlayerTo3DReticle;

	// 3Dレティクルの座標設定
	object->SetPos(pPos + offset);

	// 座標更新
	object->TransUpdate();
}

KMyMath::Vector3 Reticle3D::GetWorldPos()
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object->GetMatWorld().m[3][0];
	result.y = object->GetMatWorld().m[3][1];
	result.z = object->GetMatWorld().m[3][2];

	return result;
}
