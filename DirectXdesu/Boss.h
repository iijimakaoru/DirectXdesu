#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"
#include "KTexture.h"
#include "BossActState.h"
#include <memory>
#include <list>
#include "ViewProjection.h"

class Boss
{
public:
	static Boss* nowBoss;

	std::unique_ptr<BossActState> actState = nullptr;

	Boss(KModel* model);
	void Init();
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Damage();
};

