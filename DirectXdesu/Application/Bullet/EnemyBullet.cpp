/**
 * @file EnemyBullet.cpp
 * @brief 敵の弾
 * @author 飯島 薫
 */

#include "EnemyBullet.h"

EnemyBullet* EnemyBullet::Create(KModel* model, KGPlin* pipeline_,
	const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed)
{
	// インスタンス生成
	EnemyBullet* enemyBullet = new EnemyBullet();
	if (enemyBullet == nullptr)
	{
		return nullptr;
	}

	// 初期化
	enemyBullet->Init(model, pipeline_);
	enemyBullet->Set(pos, vec_, rot_, bulletSpeed);

	return enemyBullet;
}

void EnemyBullet::Init(KModel* model_, KGPlin* pipeline_)
{
	// モデル生成
	model = model_;

	// パイプライン生成
	pipeline = pipeline_;

	// オブジェクト生成
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline);
	object3d->LoadModel(model);
	object3d->SetScale({ 3.0f,3.0f,3.0f });
}
