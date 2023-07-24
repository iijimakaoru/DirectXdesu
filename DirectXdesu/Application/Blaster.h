#pragma once
#include "BossEnemy.h"
#include "KInput.h"

class Blaster : public BossEnemy
{
public:
	void Init(KModel* model_, const KMyMath::Vector3& initPos) override;

	void Update(ViewProjection* viewPro) override;

private:
	KInput* input = nullptr;
};

