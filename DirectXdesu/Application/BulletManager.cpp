#include "BulletManager.h"

BulletManager* BulletManager::bulletManager = nullptr;

void BulletManager::Init(KModel* playersBulletModel_, KGPlin* pipeline_)
{
	playersBulletModel = playersBulletModel_;

	pipeline = pipeline_;
}

void BulletManager::Update(ViewProjection* viewPro)
{
	// �e�̍폜
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

void BulletManager::PlayerBulletShot(const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed)
{
	// �e����
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(PlayerBullet::Create(playersBulletModel, pipeline, pos, vec_, rot_, bulletSpeed));
	// �o�^
	playerBullets.push_back(std::move(newBullet));
}

void BulletManager::EnemyBulletShot(const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed)
{
	// �e����
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(playersBulletModel, pipeline, pos, vec_, rot_, bulletSpeed));
	// �o�^
	enemyBullets.push_back(std::move(newBullet));
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
	// �v���C���[�̒e
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& playerBullet) {return playerBullet->GetIsDead(); });

	// �G�̒e
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& enemyBullet) {return enemyBullet->GetIsDead(); });
}