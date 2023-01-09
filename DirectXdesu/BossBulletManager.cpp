#include "BossBulletManager.h"

void BossBulletManager::CreatePool(KModel* model)
{
	for (int i = 0; i < BulletNum; i++)
	{
		std::unique_ptr<BossBullet> newBossBullet = std::make_unique<BossBullet>();
		newBossBullet->Init(model);
		newBossBullet->SetInfo({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0);

		bossBullets.push_back(std::move(newBossBullet));
	}
	pooled = true;
}

void BossBulletManager::AllDelete()
{
	for (std::unique_ptr<BossBullet>& bossBullet : bossBullets)
	{
		bossBullet->isDead = true;
	}
}

void BossBulletManager::Update(ViewProjection& viewProjection)
{
	for (std::unique_ptr<BossBullet>& bossBullet : bossBullets)
	{
		if (!bossBullet->IsDead())
		{
			bossBullet->Update(viewProjection);
		}
	}
}

void BossBulletManager::Draw()
{
	for (std::unique_ptr<BossBullet>& bossBullet : bossBullets)
	{
		if (!bossBullet->IsDead())
		{
			bossBullet->Draw();
		}
	}
}

void BossBulletManager::WaveBullet(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float speed, float angleY, const float lifeTime)
{
	for (std::unique_ptr<BossBullet>& bossBullet : bossBullets)
	{
		if (bossBullet->IsDead())
		{
			Vector3 velocity =
			{
				speed * cosf(angleY),
				0,
				speed * sinf(angleY),
			};

			bossBullet->SetInfo(pos, velocity, scale, rotation, lifeTime);

			bossBullet->Revive();

			return;
		}
	}
}

void BossBulletManager::AimBullet(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const Vector3& velocity, const float speed, const float lifeTime)
{
	for (std::unique_ptr<BossBullet>& bossBullet : bossBullets)
	{
		if (bossBullet->IsDead())
		{
			Vector3 moveVec =
			{
				velocity.x * speed,
				velocity.y * speed,
				velocity.z * speed,
			};

			bossBullet->SetInfo(pos, moveVec, scale, rotation, lifeTime);

			bossBullet->Revive();

			return;
		}
	}
}

BossBulletManager* BossBulletManager::GetInstance()
{
	static BossBulletManager instance;
	return &instance;
}
