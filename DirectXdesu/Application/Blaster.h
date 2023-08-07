#pragma once
#include "BossEnemy.h"
#include "KInput.h"

class Blaster : public BossEnemy
{
public:
	static Blaster* Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos, const float HP);

public:
	void Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos, const float HP) override;

	void Update(ViewProjection* viewPro) override;

private:
	KInput* input = nullptr;
};

