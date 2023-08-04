#include "AppearEnemy.h"
#include "Player.h"
#include "BulletManager.h"
#include "Ease.h"

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

	startScale = { 0,0,0 };

	endScale = { 4.0f,4.0f,4.0f };

	object3d->transform.scale = startScale;

	isAppear = true;

	easeTimer = 0;

	isDead = false;
}

void AppearEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	// 出現演出
	if (isAppear)
	{
		Appear();
	}

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

void AppearEnemy::Appear()
{
	easeTimer += 0.05f;

	object3d->transform.scale = MyMathUtility::OutQuadVec3(startScale, endScale, easeTimer);
	object3d->transform.rot.y = 
		MyMathUtility::OutQuadFloat(0.0f, 360.0f * 2.0f, easeTimer);

	if (easeTimer >= max(easeTimer, 1))
	{
		isAppear = false;
	}
}
