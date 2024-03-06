#include "CanonEnemy.h"
#include "BulletManager.h"
#include "Ease.h"
#include "Player.h"
#include "ScoreManager.h"

CanonEnemy* CanonEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_) {
	CanonEnemy* newEnemy = new CanonEnemy();
	if (newEnemy == nullptr) {
		return nullptr;
	}

	newEnemy->Init(model_, pipeline_);

	newEnemy->SetStartPos(pos_);

	newEnemy->object3d->GetTransform().SetPos(pos_);

	return newEnemy;
}

void CanonEnemy::Init(KModel* model_, KGPlin* pipeline_) {
	MobEnemy::Init(model_, pipeline_);

	isAppear = true;

	easeTimer = 0;

	object3d->GetTransform().SetScale({6.0f, 6.0f, 6.0f});

	coolTime = 30;
}

void CanonEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	// 出現演出
	if (isAppear) {
		Appear();
	} else {
		if (!isDead && !isDelete) {
			if (object3d->GetTransform().GetPos().z >= cameraPos.z) {
				Attack();
			}

			const float cameraDistance = 50.0f;

			if (object3d->GetTransform().GetPos().z <=
			    min(object3d->GetTransform().GetPos().z,
			        cameraPos.z - cameraDistance)) {
				isDelete = true;
			}
		}

		if (isDead) {
			ScoreManager::GetInstance()->AddMobScore(100);
			ScoreManager::GetInstance()->AddDestoryCount();
		}
	}

	object3d->Update(viewPro, cameraPos);
}

void CanonEnemy::Draw() { MobEnemy::Draw(); }

void CanonEnemy::Attack() { // プレイヤー情報
	assert(player);

	// クールタイム経過
	coolTimer++;

	if (coolTimer >= max(coolTimer, coolTime)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;

		// 弾のパワー
		const float bulletPower = 2.0f;

		// 自キャラのワールド座標
		KMyMath::Vector3 pPos = player->GetWorldPos();

		// ワールド座標
		KMyMath::Vector3 ePos = GetWorldPos();

		// 差分ベクトル
		KMyMath::Vector3 vec = pPos - ePos;

		// 弾生成
		BulletManager::GetInstance()->EnemyBulletShot(
		    ePos, vec, {1, 1, 1}, kBulletSpeed, bulletPower);

		// クールタイム初期化
		coolTimer = 0;
	}
}

void CanonEnemy::Appear() {
	easeTimer += 1.0f;

	object3d->GetTransform().SetPos(
	    {object3d->GetTransform().GetPos().x,
	     MyEase::InOutCubicFloat(startPos.y, startPos.y + 20, easeTimer / easeTime),
	     object3d->GetTransform().GetPos().z});

	object3d->GetTransform().SetRot(
	    {object3d->GetTransform().GetRot().x,
	     MyEase::OutCubicFloat(0, 360 * 2, easeTimer / easeTime),
	     object3d->GetTransform().GetRot().z});

	if (easeTimer >= max(easeTimer, easeTime)) {
		isAppear = false;
	}
}

void CanonEnemy::SetStartPos(const KMyMath::Vector3& startPos_) { startPos = startPos_; }
