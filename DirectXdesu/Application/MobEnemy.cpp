#include "MobEnemy.h"
#include "MyMath.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#include "Player.h"

void MobEnemy::Init(KModel* model_, KGPlin* pipeline_)
{
	// モデル生成
	model = model_;

	// パイプライン
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));

	isDead = false;
}

void MobEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	object3d->Update(viewPro);
}

void MobEnemy::Draw()
{
	if (!isDead)
	{
		object3d->Draw();
	}
}

void MobEnemy::OnCollision()
{
	ParticleManager::GetInstance()->CallExp(GetWorldPos());
	isDead = true;
}

void MobEnemy::Attack()
{
	assert(player);

	// クールタイム経過
	coolTimer++;

	if (coolTimer >= max(coolTimer, coolTime))
	{
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

		// クールタイム初期化
		coolTimer = 0;
	}
}

const KMyMath::Vector3 MobEnemy::GetWorldPos() const
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}