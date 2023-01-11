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
	void AttackHit();

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
	KTexture normal;

	ViewProjection view;

	Vector3 moveVec;

	int hp = 0;

	constexpr static int maxHP = 20;

	bool isDash = false;

	float dashTimer = 0;

	constexpr static float maxDashTimer = 10;

	Vector3 dashVec;

	float particleCoolTime = 0;

	bool isHit = false;

	bool isDamage = false;

	float damageTimer = 0;

	static Player* nowPlayer;
};

