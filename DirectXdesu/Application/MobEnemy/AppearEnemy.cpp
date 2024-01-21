#include "AppearEnemy.h"
#include "BulletManager.h"
#include "Ease.h"
#include "ScoreManager.h"
#include "Player.h"

AppearEnemy* AppearEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_) {
	// インスタンス生成
	AppearEnemy* newEnemy = new AppearEnemy();
	if (newEnemy == nullptr) {
		return nullptr;
	}

	// 初期化
	newEnemy->Init(model_, pipeline_);

	// 初期位置セット
	newEnemy->object3d->SetPos(pos_);

	return newEnemy;
}

AppearEnemy::AppearEnemy() {}

AppearEnemy::~AppearEnemy() {}

void AppearEnemy::Init(KModel* model_, KGPlin* pipeline_) {
	MobEnemy::Init(model_, pipeline_);

	startScale = {0, 0, 0};

	endScale = {4.0f, 4.0f, 4.0f};

	object3d->SetScale(startScale);

	isAppear = true;

	easeTimer = 0;

	coolTime = 60;
}

void AppearEnemy::Update(ViewProjection* viewPro_, const KMyMath::Vector3& cameraPos_) {
	// 出現演出
	if (isAppear) {
		Appear();
	} else {
		if (!isDead && !isDelete) {
			if (object3d->GetPos().z >= cameraPos_.z) {
				Attack();
			}

			const float cameraDistance = 50.0f;

			if (object3d->GetPos().z <= min(object3d->GetPos().z, cameraPos_.z - cameraDistance)) {
				isDelete = true;
			}
		}

		if (isDead) {
			ScoreManager::GetInstance()->AddMobScore(100);
			ScoreManager::GetInstance()->AddDestoryCount();
		}
	}

	object3d->Update(viewPro_);
}

void AppearEnemy::Draw() { MobEnemy::Draw(); }

void AppearEnemy::Attack() { // プレイヤー情報
	assert(player);

	// クールタイム経過
	coolTimer++;

	if (coolTimer >= max(coolTimer, coolTime)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;

		// 弾のパワー
		const float bulletPower = 1.0f;

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

void AppearEnemy::Appear() {
	easeTimer += 1.0f;

	object3d->SetScale(MyEase::OutQuadVec3(startScale, endScale, easeTimer / easeTime));
	object3d->SetRot(
	    {object3d->GetRot().x, MyEase::OutQuadFloat(0.0f, 360.0f * 2.0f, easeTimer / easeTime),
	     object3d->GetRot().z});

	if (easeTimer >= max(easeTimer, easeTime)) {
		isAppear = false;
	}
}
