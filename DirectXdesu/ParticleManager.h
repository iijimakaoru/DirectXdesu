#pragma once
#include "Particle.h"
#include <list>
#include <memory>
#include "KGPlin.h"

class ParticleManager
{
public:
	bool IsPoolCreated() {
		return pooled;
	}
	void CreatePool(KModel* model, KGPlin* pipeline);
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
	void RightWave(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed);
	void LeftWave(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed);
	void Splash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed, const Vector3& vec);
	void TestSplash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float speed, float angleY, const float lifeTime);
	void Taihun(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float speed, float angleY, const float lifeTime);

private:
	std::list<std::unique_ptr<Particle>> particles;
	std::list<std::unique_ptr<Particle2>> particles2;
	bool pooled = false;

public:
	static ParticleManager* GetInstance();
	static void DeleteInstance();

private:
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;
};

