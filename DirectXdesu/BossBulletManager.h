#pragma once
#include "BossBullet.h"
#include <list>
#include <memory>
#include "KModel.h"
#include "ViewProjection.h"

class BossBulletManager
{
public:
	bool IsPoolCreated() {
		return pooled;
	}
	void CreatePool(KModel* model);
	void AllDelete();
	void Update(ViewProjection& viewProjection);
	void Draw();
	void WaveBullet(const Vector3& pos, 
		const Vector3& scale, 
		const Vector3& rotation, 
		const float speed, 
		float angleY, 
		const float lifeTime);

private:
	std::list<std::unique_ptr<BossBullet>> bossBullets;
	int shotNum = 0;
	float angle = 0;
	bool pooled = false;

public:
	static BossBulletManager* GetInstance();

private:
	BossBulletManager() = default;
	~BossBulletManager() = default;
	BossBulletManager(const BossBulletManager&) = delete;
	const BossBulletManager& operator=(const BossBulletManager&) = delete;
};

