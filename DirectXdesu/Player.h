#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"
#include "KTexture.h"
#include "ViewProjection.h"

class Player
{
public:
	void Init(KModel* model);
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Damage();

	// オブジェクト
	KObject3d object;

	// モデル
	KModel* model_;

	// 速度
	float speed;

	// 
	bool isJump;
	float jumpPower;
	const int jumpPowerMax = 5;

	Vector2 rStick;

	Vector2 lStick;

	// テクスチャ
	KTexture texture_;

	ViewProjection view;

	int hp = 0;

	constexpr static int maxHP = 20;

	static Player* nowPlayer;
};

