#pragma once
#include "Particle.h"
#include <list>
#include <memory>

class ParticleManager
{
public:
	bool IsPoolCreated() {
		return pooled;
	}
	void CreatePool(KModel* model);
	void AllDelete();
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Explosion(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed);
	void Dash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed, const Vector3& moveVec);
	void DashWave(const Vector3& pos,
		const Vector3& scale,
		const Vector3& rotation,
		const float speed,
		const Vector3& playerVec,
		float angleZ,
		const float lifeTime);

private:
	std::list<std::unique_ptr<Particle>> particles;
	bool pooled = false;

public:
	static ParticleManager* GetInstance();

private:
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;
};

