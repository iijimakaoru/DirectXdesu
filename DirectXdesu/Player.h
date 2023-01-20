#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"
#include "KTexture.h"
#include "ViewProjection.h"

class Player
{
public:
	Player(KModel* model);
	void Init();
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Damage();
	void AttackHit();

	static Player* nowPlayer;
};

