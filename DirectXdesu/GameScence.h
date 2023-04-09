#pragma once
#include "KDirectXCommon.h"
#include "KObject3d.h"
#include "ViewProjection.h"
#include "KModel.h"
#include "PipelineSet.h"
#include "Sprite.h"
#include "DebugText.h"
#include "KTexture.h"
#include "Sound.h"
#include "KGPlin.h"
#include "KShader.h"
#include <imgui.h>

#include "Camera.h"

#include "Player.h"

#include "CollisionPrimitive.h"

#include <array>

#include "ParticleManager.h"

class GameScence
{
public:
	enum class CollisionMode
	{
		Sphere_Plane,
		Sphere_Triangle,
		Ray_Plane,
		Ray_Triangle,
	};

	GameScence()
	{
		LoadResources();
		Init();
	};
	~GameScence();
	void LoadResources();
	void Init();
	void Update();
	void Draw();

private:
	// パイプライン
	std::unique_ptr<KGPlin> objPipeline;

	// シェーダー
	KShader objShader;

	// モデル
	std::unique_ptr<KModel> triangle;
	std::unique_ptr<KModel> cube;
	std::unique_ptr<KModel> hoge;
	std::unique_ptr<KModel> SkyBox;

	// テクスチャ
	KTexture mario;
	KTexture haikei;

	// 3Dオブジェクト
	std::unique_ptr<KObject3d> obj;
	std::unique_ptr<KObject3d> skydorm;
	std::unique_ptr<KObject3d> stage;
	std::unique_ptr<KObject3d> testTriangle;

	// プレイヤー
	Player player;

	// スプライト
	std::unique_ptr<Sprite> sprite;
	SpriteCommon spriteCommon;

	Vector3 center = { 0,0,1 };

	float rSpeed = -0.02f;
	float gSpeed = 0.02f;
	float bSpeed = -0.02f;
	float aSpeed = -0.02f;

	std::unique_ptr<Sound> sound;

	SoundData soundData1;
	SoundData soundData2;
	SoundData soundData3;

	SpriteInfo sprites[2];

	std::unique_ptr<DebugText> debugtext;

	Vector3 vec = { 1,1,1 };

	float angle = 0;

	float hogeSpeed = 0;

	float speedLevel = 1;

	float speed = 0;

	float hogeLifeTime = 0;

	float hogeAngle = 0;

	float hogeRot = 0;

	float hogeCooltime = 0;

	float hogehoge = 0;

	bool isDebug = true;

	bool isChange = false;

	static const int MaxHoge = 2;
	std::array<SpriteInfo, MaxHoge> spriteHoge;

	Camera* camera = nullptr;

	bool isWave = false;
	bool isTaihun = false;

	CollisionMode colMode = CollisionMode::Sphere_Plane;

	/// 当たり判定
	// 球
	Sphere sphere;
	// 平面
	Plane plane;
	// レイ
	Ray ray;
	// 三角形
	ColTriangle colTriangle;

	bool isHit = false;

	BillParticleManager* partMan = nullptr;
};

