/**
 * @file BulletManager.cpp
 * @brief 弾のマネージャー
 * @author 飯島 薫
 */

#include "BulletManager.h"
#include "ModelManager.h"
#include "PipelineManager.h"

BulletManager* BulletManager::bulletManager = nullptr;

void BulletManager::Init() {
	pipeline = PipelineManager::GetInstance()->GetObjPipeline();
	for (size_t i = 0; i < 16; i++) {
		unitLazers[i].reset(UnitLazer::Create(
		    ModelManager::GetInstance()->GetModels("Lazer"), pipeline, {0.0f, 0.0f, 0.0f},
		    {0.0f, 0.0f, 90.0f}));
	}
}

void BulletManager::Update(ViewProjection* viewPro_) {
	// 弾の削除
	DeleteBullet();

	for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets) {
		playerBullet->Update(viewPro_);
	}

	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets) {
		enemyBullet->Update(viewPro_);
	}

	for (std::unique_ptr<Bom>& bom : boms) {
		bom->Update(viewPro_);
	}

	for (std::unique_ptr<UnitLazer>& unitLazer : unitLazers) {
		unitLazer->Update(viewPro_);
	}
}

void BulletManager::Draw() {
	for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets) {
		playerBullet->Draw();
	}

	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets) {
		enemyBullet->Draw();
	}

	for (std::unique_ptr<Bom>& bom : boms) {
		bom->Draw();
	}

	for (std::unique_ptr<UnitLazer>& unitLazer : unitLazers) {
		unitLazer->Draw();
	}
}

void BulletManager::PlayerBulletShot(
    const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
    const float bulletSpeed_, const float BulletPower_) {
	// 弾生成
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(PlayerBullet::Create(
	    ModelManager::GetInstance()->GetModels("P_Bullet"), pipeline, pos, vec_, rot_, bulletSpeed_,
	    BulletPower_));
	// 登録
	playerBullets.push_back(std::move(newBullet));
}

void BulletManager::EnemyBulletShot(
    const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
    const float bulletSpeed_, const float BulletPower_) {
	// 弾生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(
	    ModelManager::GetInstance()->GetModels("E_Bullet"), pipeline, pos, vec_, rot_, bulletSpeed_,
	    BulletPower_));
	// 登録
	enemyBullets.push_back(std::move(newBullet));
}

void BulletManager::BomShot(
    const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
    const float bulletSpeed_) {
	std::unique_ptr<Bom> newBom;
	newBom.reset(Bom::Create(
	    ModelManager::GetInstance()->GetModels("P_Bullet"),
	    ModelManager::GetInstance()->GetModels("Explotion"), pipeline, pos_, vec_, rot_,
	    bulletSpeed_));
	boms.push_back(std::move(newBom));
}

void BulletManager::UnitLazerSet(
    const KMyMath::Vector3& pos_, const KMyMath::Vector3& rot_, uint32_t& i) {
	unitLazers[i]->Set(pos_, rot_);
}

void BulletManager::UnitLazerCall() {
	for (std::unique_ptr<UnitLazer>& unitLazer : unitLazers) {
		unitLazer->SetIsDead(false);
	}
}

void BulletManager::UnitLazerDelete() {
	for (std::unique_ptr<UnitLazer>& unitLazer : unitLazers) {
		unitLazer->SetIsDead(true);
	}
}

BulletManager* BulletManager::GetInstance() {
	static BulletManager instance;
	return &instance;
}

void BulletManager::AllBulletDelete() {
	for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets) {
		playerBullet->SetIsDead(true);
	}

	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets) {
		enemyBullet->SetIsDead(true);
	}

	for (std::unique_ptr<Bom>& bom : boms) {
		bom->SetIsDead(true);
	}
}

void BulletManager::Delete() { delete bulletManager; }

const std::list<std::unique_ptr<PlayerBullet>>& BulletManager::GetPlayerBullets() const {
	return playerBullets;
}

const std::list<std::unique_ptr<EnemyBullet>>& BulletManager::GetEnemyBullets() const {
	return enemyBullets;
}

const std::list<std::unique_ptr<Bom>>& BulletManager::GetBoms() const { return boms; }

const KMyMath::Vector3 BulletManager::GetLazersPos(size_t i) const {
	return unitLazers[i]->GetWorldPos();
}

void BulletManager::DeleteBullet() {
	// プレイヤーの弾
	playerBullets.remove_if(
	    [](std::unique_ptr<PlayerBullet>& playerBullet_) { return playerBullet_->GetIsDead(); });

	// 敵の弾
	enemyBullets.remove_if(
	    [](std::unique_ptr<EnemyBullet>& enemyBullet_) { return enemyBullet_->GetIsDead(); });

	// ボム
	boms.remove_if([](std::unique_ptr<Bom>& bom_) { return bom_->GetIsDead(); });
}
