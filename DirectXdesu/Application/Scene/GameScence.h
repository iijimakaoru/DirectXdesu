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

#include "MobEnemy.h"

#include "Ground.h"

#include "RailCamera.h"

class GameScence : public BaseScene
{
public:
	GameScence(){};
	~GameScence();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Final()override;

private:
	// 衝突判定
	void CheckAllCollisions();

	// 敵発生データの読み込み
	void LoadEnemyPopData();

	// 敵発生コマンドの更新
	void UpdateEnemyPopCommands();

private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<RailCamera> camera = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> objPipeline;
	std::unique_ptr<KGPlin> spritePipeline;

	// シェーダー
	KShader objShader;
	KShader spriteShader;

	// テクスチャ
	KTexture mario;
	KTexture haikei;

	SoundData soundData1;
	SoundData soundData2;
	SoundData soundData3;

	bool isDebug = true;

	// スプライト
	std::unique_ptr<Sprite> sprite = nullptr;
	KMyMath::Vector2 spritePos = { 0,0 };
	KMyMath::Vector2 spriteSize = { 100,100 };
	float spriteRot = 0;
	KMyMath::Vector4 spriteColor = { 1.0f,1.0f,1.0f,1.0f };
	bool spriteFlipX, spriteFlipY = false;

	// プレイヤー
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<KModel> playerModel;

	// 雑魚
	std::list<std::unique_ptr<MobEnemy>> mobEnemys;
	std::unique_ptr<KModel> mobEnemysModel;

	// 簡易地面
	std::unique_ptr<Ground> ground = nullptr;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	bool isStand = false;
	int32_t waitTimer = 0;
};

