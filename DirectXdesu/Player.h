#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"

class Player
{
public:
	void Init(KModel* model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw();

	KObject3d object;

	KModel* model_;

	float speed;

	bool isJump;

	float jumpPower;

	const int jumpPowerMax = 5;

	Vector2 stick;

	static Player* nowPlayer;
};

