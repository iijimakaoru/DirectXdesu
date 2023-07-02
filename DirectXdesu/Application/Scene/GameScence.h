#pragma once
#include "KDirectXCommon.h"
#include "KObject3d.h"
#include "Sprite.h"
#include "KTexture.h"
#include "Sound.h"
#include "KGPlin.h"
#include "KShader.h"
#include <imgui.h>

#include "Camera.h"

#include <array>

#include "FbxObject3D.h"

#include "KModel.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Player.h"

#include "Bullet.h"

class GameScence : public BaseScene
{
public:
	GameScence()
	{
		LoadResources();
		Init();
	};
	~GameScence();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Final()override;

private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> objPipeline;
	std::unique_ptr<KGPlin> spritePipeline;

	// シェーダー
	KShader objShader;
	KShader spriteShader;

	// テクスチャ
	KTexture mario;
	KTexture haikei;

	std::unique_ptr<Sound> sound;

	SoundData soundData1;
	SoundData soundData2;
	SoundData soundData3;

	bool isDebug = true;

	// スプライト
	Sprite* sprite = nullptr;
	KMyMath::Vector2 spritePos = { 0,0 };
	KMyMath::Vector2 spriteSize = { 100,100 };
	float spriteRot = 0;
	KMyMath::Vector4 spriteColor = { 1.0f,1.0f,1.0f,1.0f };
	bool spriteFlipX, spriteFlipY = false;

	// プレイヤー
	std::unique_ptr<Player> player = nullptr;
};

