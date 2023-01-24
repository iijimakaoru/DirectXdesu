#pragma once
#include "KDirectXCommon.h"
#include "KInput.h"
#include "KObject3d.h"
#include "ViewProjection.h"
#include "KModel.h"
#include "PipelineSet.h"
#include "Sprite.h"
#include "DebugText.h"
#include "KTexture.h"
#include "Sound.h"

class GameScence
{
public:
	GameScence(){};
	~GameScence(){};
	void LoadResouce();
	void Init();
	void Update();
	void Draw();

private:
	// モデル
	std::unique_ptr<KModel> triangle;
	std::unique_ptr<KModel> cube;

	// テクスチャ
	std::unique_ptr<KTexture> mario;
	std::unique_ptr<KTexture> haikei;

	// ビュープロジェクション
	ViewProjection viewProjection;

	// 3Dオブジェクト
	std::unique_ptr<KObject3d> obj;

	// スプライト
	std::unique_ptr<Sprite> sprite;
	SpriteCommon spriteCommon;

	Vector3 center = { 0,0,1 };

	float rSpeed = -0.02f;
	float gSpeed = 0.02f;
	float bSpeed = -0.02f;
	float aSpeed = -0.02f;

	std::unique_ptr<Sound> sound;

	SpriteInfo sprites[2];

	std::unique_ptr<DebugText> debugtext;

	Vector3 vec = { 1,1,1 };

	float angle = 0;

	float hogeSpeed = 0;

	float speedLevel = 1;

	float hogeLifeTime = 0;

	float hogeAngle = 0;

	float hogeRot = 0;

	float hogeCooltime = 0;
};

