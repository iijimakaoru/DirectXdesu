#include "AppearEnemy.h"
#include "Player.h"
#include "BulletManager.h"

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
		if (KInput::GetInstance()->IsTrigger(DIK_2))
		{
			Attack();
		}

		if (object3d->transform.pos.z <= min(object3d->transform.pos.z, cameraPos.z))
		{
			isDead = true;
		}
	}

	object3d->Update(viewPro);
}

void AppearEnemy::Attack()
{
	assert(player);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標
	KMyMath::Vector3 pPos = player->GetWorldPos();

	// ワールド座標
	KMyMath::Vector3 ePos = GetWorldPos();

	// 差分ベクトル
	KMyMath::Vector3 vec = pPos - ePos;

	// 弾生成
	BulletManager::GetInstance()->EnemyBulletShot(ePos, vec, { 1,1,1 }, kBulletSpeed);
}
