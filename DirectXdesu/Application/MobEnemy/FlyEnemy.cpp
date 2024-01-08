#include "FlyEnemy.h"
#include "Ease.h"
#include "ScoreManager.h"

FlyEnemy* FlyEnemy::Create(
    KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos_,
    const KMyMath::Vector2& endPos_, const float& speed_) {
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
	newEnemy->SetSpeed(speed_);
	newEnemy->object3d->SetPos(startPos_);

	return newEnemy;
}

void FlyEnemy::Init(KModel* model_, KGPlin* pipeline_) {
	MobEnemy::Init(model_, pipeline_);

	object3d->SetScale({4.0f, 4.0f, 4.0f});

	isAppear = true;

	easeTimer = 0;

	coolTime = 120;
}

void FlyEnemy::Update(ViewProjection* viewPro_, const KMyMath::Vector3& cameraPos_) {
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

	object3d->AddSetPos({0, 0, moveSpeed});

	object3d->Update(viewPro_);
}

void FlyEnemy::Draw() { MobEnemy::Draw(); }

void FlyEnemy::Attack() { MobEnemy::Attack(); }

void FlyEnemy::Appear() {
	easeTimer += 1.0f;

	object3d->SetPos(
	    {MyEase::InCubicFloat(startPos.x, endPos.x, easeTimer / easeTime),
	     MyEase::InCubicFloat(startPos.y, endPos.y, easeTimer / easeTime), object3d->GetPos().z});

	if (easeTimer >= max(easeTimer, easeTime)) {
		isAppear = false;
	}
}

void FlyEnemy::SetStartPos(const KMyMath::Vector3& startPos_) { startPos = startPos_; }

void FlyEnemy::SetEndPos(const KMyMath::Vector2& endPos_) { endPos = endPos_; }

void FlyEnemy::SetSpeed(const float& speed_) { moveSpeed = speed_; }
