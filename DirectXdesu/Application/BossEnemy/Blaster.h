#pragma once
#include "BossEnemy.h"
#include "KInput.h"

class Blaster : public BossEnemy
{
public:
	static Blaster* Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos,
		const float HP, KGPlin* spritePipeline_);

public:
	void Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos,
		const float HP_, KGPlin* spritePipeline_) override;

	void Update(ViewProjection* viewPro) override;

	bool CollisionCheck(const KMyMath::Vector3& posA, const KMyMath::Vector3& posB) override;

private:
	KInput* input = nullptr;
};

