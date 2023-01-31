#include "ParticleManager.h"
#include "MyMath.h"

void ParticleManager::CreatePool(KModel* model, KGPlin* pipeline)
{
	for (int i = 0; i < 200; i++)
	{
		std::unique_ptr<Particle> newParticle = std::make_unique<Particle>();
		newParticle->Init(model,pipeline);
		newParticle->SetInfo({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0);

		particles.push_back(std::move(newParticle));

		std::unique_ptr<Particle2> newParticle2 = std::make_unique<Particle2>();
		newParticle2->Init(model, pipeline);
		newParticle2->SetInfo({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0);

		particles2.push_back(std::move(newParticle2));
	}
	pooled = true;
}

void ParticleManager::AllDelete()
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		particle->isDead = true;
	}

	for (std::unique_ptr<Particle2>& particle2 : particles2)
	{
		particle2->isDead = true;
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

	for (std::unique_ptr<Particle2>& particle2 : particles2)
	{
		if (!particle2->IsDead())
		{
			particle2->Update(viewProjection);
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

	for (std::unique_ptr<Particle2>& particle2 : particles2)
	{
		if (!particle2->IsDead())
		{
			particle2->Draw();
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

void ParticleManager::Dash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed, const Vector3& moveVec)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				MyMath::GetInstance()->GetRand(-speed, speed),
				MyMath::GetInstance()->GetRand(-speed, speed),
				-speed,
			};

			particle->SetInfo(pos, velocity * moveVec, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}
	}
}

void ParticleManager::DashWave(const Vector3& pos,
	const Vector3& scale,
	const Vector3& rotation,
	const float speed,
	const Vector3& playerVec,
	float angleZ,
	const float lifeTime)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				speed * cosf(angleZ),
				speed * sinf(angleZ),
				0,
			};

			particle->SetInfo(pos, velocity * playerVec, scale, rotation, lifeTime);

			particle->Revive();

			return;
		}
	}
}

void ParticleManager::RightWave(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				MyMath::GetInstance()->GetRand(speed / 2, speed),
				speed,
				-speed,
			};

			particle->SetInfo(pos, velocity, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}

	}
}

void ParticleManager::LeftWave(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				-MyMath::GetInstance()->GetRand(speed / 2 ,speed),
				speed,
				-speed,
			};

			particle->SetInfo(pos, velocity, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}

	}
}

void ParticleManager::Splash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed, const Vector3& vec)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				MyMath::GetInstance()->GetRand(-speed,speed) * vec.x,
				(MyMath::GetInstance()->GetRand(speed / 2, speed) * 4) * vec.y,
				MyMath::GetInstance()->GetRand(-speed,speed) * vec.z,
			};

			particle->SetInfo(pos, velocity, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}

	}
}

void ParticleManager::TestSplash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float speed, float angleY, const float lifeTime)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				speed * cosf(angleY),
				speed * 2,
				speed * sinf(angleY),
			};

			particle->SetInfo(pos, velocity, scale, rotation, lifeTime);

			particle->Revive();

			return;
		}
	}
}

void ParticleManager::Taihun(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float speed, float angleY, const float lifeTime)
{
	for (std::unique_ptr<Particle2>& particle2 : particles2)
	{
		if (particle2->IsDead())
		{
			Vector3 velocity =
			{
				speed * cosf(angleY),
				speed * sinf(angleY),
				speed * 4,
			};

			particle2->SetInfo(pos, velocity, scale, rotation, lifeTime);

			particle2->Revive();

			return;
		}
	}
}

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::DeleteInstance()
{
	delete ParticleManager::GetInstance();
}
