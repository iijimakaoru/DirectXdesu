#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"
#include "KTexture.h"
#include "ViewProjection.h"
#include "KGPlin.h"
#include "KShader.h"

class Player
{
public:
	Player();
	void Init(KModel* model, KGPlin* pipeline);
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Damage();
	void AttackHit();

	Vector3& GetPos() { return object->transform.pos; }

private:
	std::unique_ptr<KObject3d> object;
	std::unique_ptr<KModel> model;

public:
	static Player* nowPlayer;
};

