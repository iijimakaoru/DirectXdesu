#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"
#include "KTexture.h"
#include "ViewProjection.h"
#include "Vector3.h"

class Player
{
public:
	Player(KModel* model);
	void Init();
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Damage();
	void AttackHit();

	// オブジェクト
	KObject3d object;

	// 弾
	std::unique_ptr<KObject3d> bulletObj1;
	std::unique_ptr<KObject3d> bulletObj2;
	std::unique_ptr<KObject3d> bulletObj3;
	bool isDrawBullet = false;

	Vector3 start;
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;
	Vector3 end;

	float maxTime = 30;
	float nowTime = 0;

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

	constexpr static int maxHP = 3;

	bool isDash = false;

	float dashTimer = 0;

	constexpr static float maxDashTimer = 10;

	Vector3 dashVec;

	float particleCoolTime = 0;

	bool isHit = false;

	float hitTimer = 0;

	bool isDamage = false;

	float damageTimer = 0;

	bool isAlive = false;

	static Player* nowPlayer;
};

