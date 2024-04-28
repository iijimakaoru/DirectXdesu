#include "FlyEnemy.h"
#include "BulletManager.h"
#include "Ease.h"
#include "Player.h"
#include "ScoreManager.h"

FlyEnemy* FlyEnemy::Create(
    KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos_,
    const KMyMath::Vector2& endPos_, const float& speed_, const uint32_t& lifeTime_,
    const KMyMath::Vector2& wStartPos_, const KMyMath::Vector2& wEndPos_) {
	// インスタンス生成
	FlyEnemy* newEnemy = new FlyEnemy();
	if (newEnemy == nullptr) {
		return nullptr;
	}

	// 初期化
	newEnemy->Init(model_, pipeline_);

	// 初期位置セット
	newEnemy->SetStartPos(startPos_);
	newEnemy->SetEndPos(endPos_);
	newEnemy->SetWStartPos(wStartPos_);
	newEnemy->SetWEndPos(wEndPos_);
	newEnemy->SetSpeed(speed_);
	newEnemy->SetLifeTime(lifeTime_);
	newEnemy->object3d->GetTransform().SetPos(startPos_);

	return newEnemy;
}

void FlyEnemy::Init(KModel* model_, KGPlin* pipeline_) {
	MobEnemy::Init(model_, pipeline_);

	object3d->GetTransform().SetScale({4.0f, 4.0f, 4.0f});

	isAppear = true;

	easeTimer = 0;

	coolTime = 120;
}

void FlyEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	// 出現演出
	switch (enemyMode) {
	case FlyEnemy::AppearMode:
		Appear();
		break;
	case FlyEnemy::BattleMode:
		Battle(cameraPos);
		break;
	case FlyEnemy::WithdrawalMode:
		Withdrawal();
		break;
	default:
		break;
	}
	
	if (isDead) {
		ScoreManager::GetInstance()->AddMobScore(100);
		ScoreManager::GetInstance()->AddDestoryCount();
	}

	object3d->GetTransform().AddSetPos({0, 0, moveSpeed});

	object3d->Update(viewPro, cameraPos);
}

void FlyEnemy::Draw() { MobEnemy::Draw(); }

void FlyEnemy::Attack() {
	// プレイヤー情報
	assert(player);

	// クールタイム経過
	coolTimer++;

	if (coolTimer >= max(coolTimer, coolTime)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;

		// 弾のパワー
		const float bulletPower = 5.0f;

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

void FlyEnemy::Appear() {
	easeTimer += 1.0f;

	object3d->GetTransform().SetPos(
	    {MyEase::InCubicFloat(startPos.x, endPos.x, easeTimer / easeTime),
	     MyEase::InCubicFloat(startPos.y, endPos.y, easeTimer / easeTime),
	     object3d->GetTransform().GetPos().z});

	if (easeTimer >= max(easeTimer, easeTime)) {
		enemyMode = BattleMode;
		easeTimer = 0;
	}
}

void FlyEnemy::Battle(const KMyMath::Vector3& cameraPos) {
	if (!isDead && !isDelete) {
		lifeTimer++;
		if (object3d->GetTransform().GetPos().z >= cameraPos.z) {
			Attack();
		}

		if (lifeTimer >= lifeTime) {
			enemyMode = WithdrawalMode;
		}
	}
}

void FlyEnemy::Withdrawal() {
	easeTimer += 1.0f;

	object3d->GetTransform().SetPos(
	    {MyEase::InCubicFloat(wStartPos.x, wEndPos.x, easeTimer / easeTime),
	     MyEase::InCubicFloat(wStartPos.y, wEndPos.y, easeTimer / easeTime),
	     object3d->GetTransform().GetPos().z});

	if (easeTimer >= max(easeTimer, easeTime)) {
		isDelete = true;
	}
}

void FlyEnemy::SetStartPos(const KMyMath::Vector3& startPos_) { startPos = startPos_; }

void FlyEnemy::SetWStartPos(const KMyMath::Vector2& wStartPos_) { wStartPos = wStartPos_; }

void FlyEnemy::SetEndPos(const KMyMath::Vector2& endPos_) { endPos = endPos_; }

void FlyEnemy::SetWEndPos(const KMyMath::Vector2& wEndPos_) { wEndPos = wEndPos_; }

void FlyEnemy::SetSpeed(const float& speed_) { moveSpeed = speed_; }

void FlyEnemy::SetLifeTime(const uint32_t& lifeTime_) { lifeTime = lifeTime_; }
