#include "ParticleManager.h"
#include "MyMath.h"

void ParticleManager::CreatePool(KModel* model)
{
	for (int i = 0; i < 100; i++)
	{
		std::unique_ptr<Particle> newParticle = std::make_unique<Particle>();
		newParticle->Init(model);
		newParticle->SetInfo({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0);

		particles.push_back(std::move(newParticle));
	}
	pooled = true;
}

void ParticleManager::AllDelete()
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		particle->isDead = true;
	}
}

void ParticleManager::Update(ViewProjection& viewProjection)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (!particle->IsDead())
		{
			particle->Update(viewProjection);
		}
	}
}

void ParticleManager::Draw()
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (!particle->IsDead())
		{
			particle->Draw();
		}
	}
}

void ParticleManager::Explosion(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				MyMath::GetInstance()->GetRand(-speed,speed) * cosf(MyMath::GetInstance()->GetRand(0.0f,4.0f * PI)),
				MyMath::GetInstance()->GetRand(-speed,speed) * sinf(MyMath::GetInstance()->GetRand(0.0f,4.0f * PI)),
				MyMath::GetInstance()->GetRand(-speed,speed) * cosf(MyMath::GetInstance()->GetRand(0.0f,4.0f * PI)),
			};
			particle->SetInfo(pos, velocity, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}
	}
}

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}
