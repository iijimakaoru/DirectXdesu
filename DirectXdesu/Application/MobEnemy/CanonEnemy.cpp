#include "CanonEnemy.h"
#include "Ease.h"
#include "ScoreManager.h"

CanonEnemy* CanonEnemy::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_) {
	CanonEnemy* newEnemy = new CanonEnemy();
	if (newEnemy == nullptr) {
		return nullptr;
	}

	newEnemy->Init(model_, pipeline_);

	newEnemy->SetStartPos(pos_);

	newEnemy->object3d->SetPos(pos_);

	return newEnemy;
}

void CanonEnemy::Init(KModel* model_, KGPlin* pipeline_) {
	MobEnemy::Init(model_, pipeline_);

	isAppear = true;

	easeTimer = 0;

	object3d->SetScale({6.0f, 6.0f, 6.0f});

	coolTime = 30;
}

void CanonEnemy::Update(ViewProjection* viewPro_, const KMyMath::Vector3& cameraPos_) {
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

void CanonEnemy::Draw() { MobEnemy::Draw(); }

void CanonEnemy::Attack() { MobEnemy::Attack(); }

void CanonEnemy::Appear() {
	easeTimer += 1.0f;

	object3d->SetPos(
	    {object3d->GetPos().x,
	     MyEase::InOutCubicFloat(startPos.y, startPos.y + 20, easeTimer / easeTime),
	     object3d->GetPos().z});

	object3d->SetRot(
	    {object3d->GetRot().x, MyEase::OutCubicFloat(0, 360 * 2, easeTimer / easeTime),
	     object3d->GetRot().z});

	if (easeTimer >= max(easeTimer, easeTime)) {
		isAppear = false;
	}
}

void CanonEnemy::SetStartPos(const KMyMath::Vector3& startPos_) { startPos = startPos_; }
