#include "AppearEnemy.h"

AppearEnemy* AppearEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos)
{
	// インスタンス生成
	AppearEnemy* appearEnemy = new AppearEnemy();
	if (appearEnemy == nullptr)
	{
		return nullptr;
	}

	// 初期化
	appearEnemy->Init(model_,pipeline_);

	// 初期位置セット
	appearEnemy->object3d->SetPos(pos);

	return appearEnemy;
}

void AppearEnemy::Init(KModel* model_, KGPlin* pipeline_)
{
	// モデル生成
	model = model_;

	// パイプライン
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->transform.scale = { 4.0f,4.0f,4.0f };
}

void AppearEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	if (!isDead)
	{
		if (object3d->transform.pos.z <= min(object3d->transform.pos.z, cameraPos.z))
		{
			isDead = true;
		}
	}

	object3d->Update(viewPro);
}
