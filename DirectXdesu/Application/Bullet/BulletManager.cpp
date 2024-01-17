/**
 * @file BulletManager.cpp
 * @brief 弾のマネージャー
 * @author 飯島 薫
 */

#include "BulletManager.h"

BulletManager* BulletManager::bulletManager = nullptr;

void BulletManager::Init(KGPlin* pipeline_) {
	playersBulletModel = std::make_unique<MtlObj>("playerBullet");
	playersBulletModel->CreateModel();

	enemysBulletModel = std::make_unique<MtlObj>("EnemyBullet");
	enemysBulletModel->CreateModel();

	bomsModel = std::make_unique<MtlObj>("playerBullet");
	bomsModel->CreateModel();

	expsModel = std::make_unique<MtlObj>("Explosion");
	expsModel->CreateModel();

	pipeline = pipeline_;
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
}

void BulletManager::PlayerBulletShot(
    const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
    const float bulletSpeed_) {
	// 弾生成
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(
	    PlayerBullet::Create(playersBulletModel.get(), pipeline, pos, vec_, rot_, bulletSpeed_));
	// 登録
	playerBullets.push_back(std::move(newBullet));
}

void BulletManager::EnemyBulletShot(
    const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
    const float bulletSpeed_) {
	// 弾生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(
	    EnemyBullet::Create(enemysBulletModel.get(), pipeline, pos, vec_, rot_, bulletSpeed_));
	// 登録
	enemyBullets.push_back(std::move(newBullet));
}

void BulletManager::BomShot(
    const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
    const float bulletSpeed_) {
	std::unique_ptr<Bom> newBom;
	newBom.reset(
	    Bom::Create(bomsModel.get(), expsModel.get(), pipeline, pos_, vec_, rot_, bulletSpeed_));
	boms.push_back(std::move(newBom));
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

void BulletManager::DeleteBullet() {
	// プレイヤーの弾
	playerBullets.remove_if(
	    [](std::unique_ptr<PlayerBullet>& playerBullet_) { return playerBullet_->GetIsDead(); });

	// 敵の弾
	enemyBullets.remove_if(
	    [](std::unique_ptr<EnemyBullet>& enemyBullet_) { return enemyBullet_->GetIsDead(); });

	boms.remove_if(
	    [](std::unique_ptr<Bom>& bom_) { return bom_->GetIsDead(); });
}
