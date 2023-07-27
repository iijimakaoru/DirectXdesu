#include "BulletManager.h"

BulletManager* BulletManager::bulletManager = nullptr;

void BulletManager::Update(ViewProjection* viewPro)
{
	// íeÇÃçÌèú
	DeleteBullet();

	for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
		playerBullet->Update(viewPro);
	}

	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
	{
		enemyBullet->Update(viewPro);
	}
}

void BulletManager::Draw()
{
	for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
		playerBullet->Draw();
	}

	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
	{
		enemyBullet->Draw();
	}
}

void BulletManager::PlayerBulletShot(KModel* model, KGPlin* pipeline_,
	const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed)
{
	// íeê∂ê¨
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(PlayerBullet::Create(model, pipeline_, pos, vec_, rot_, bulletSpeed));
	// ìoò^
	playerBullets.push_back(std::move(newBullet));
}

void BulletManager::EnemyBulletShot()
{
}

BulletManager* BulletManager::GetInstance()
{
	static BulletManager instance;
	return &instance;
}

void BulletManager::Delete()
{
	delete bulletManager;
}

const std::list<std::unique_ptr<PlayerBullet>>& BulletManager::GetPlayerBullets() const
{
	return playerBullets;
}

const std::list<std::unique_ptr<EnemyBullet>>& BulletManager::GetEnemyBullets() const
{
	return enemyBullets;
}

void BulletManager::DeleteBullet()
{
	// ÉvÉåÉCÉÑÅ[ÇÃíe
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& playerBullet) {return playerBullet->GetIsDead(); });

	// ìGÇÃíe
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& enemyBullet) {return enemyBullet->GetIsDead(); });
}
