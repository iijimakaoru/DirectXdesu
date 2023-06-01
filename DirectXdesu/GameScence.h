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

#include <array>

#include "FbxObject3D.h"

#include "KModel.h"

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
	std::unique_ptr<KGPlin> fbxPipeline;

	// シェーダー
	KShader objShader;
	KShader fbxShader;

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

	bool isDebug = true;

	Camera* camera = nullptr;

	// Fbx関連
	FbxModel* fbxModel1 = nullptr;
	FbxObject3D* object1 = nullptr;
};

