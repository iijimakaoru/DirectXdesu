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
	enum class ControllerMode
	{
		KeyBord,
		GamePad,
	};

	Player();
	void Init(KModel* model, KGPlin* pipeline);
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Damage();
	void AttackHit();

	Vector3& GetPos() { return object->transform.pos; }

	void SetPos(Vector3 pos_)
	{
		object->transform.pos = pos_;
	}

private:
	std::unique_ptr<KObject3d> object;
	std::unique_ptr<KModel> model;

	ControllerMode mode = ControllerMode::KeyBord;

public:
	static Player* nowPlayer;
};

