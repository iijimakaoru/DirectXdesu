#pragma once
#include "KDirectXCommon.h"
#include "KObject3d.h"
#include "Sprite.h"
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

#include "Warning.h"

#include "BossEnemy.h"

#include "BulletManager.h"

#include "SkyBox.h"

#include "ParticleManager.h"

class GameScence : public BaseScene
{
public:
	GameScence(){};
	~GameScence();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Final()override;

private:
	// 衝突判定
	void CheckAllCollisions();

	// 敵発生データの読み込み
	void LoadEnemyPopData();

	// 敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	// ボスバトル開始
	void BossBattleStart();

private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<RailCamera> camera = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> objPipeline;
	std::unique_ptr<KGPlin> spritePipeline;
	std::unique_ptr<KGPlin> fbxPipeline;

	// シェーダー
	KShader objShader;
	KShader spriteShader;
	KShader fbxShader;

	// サウンド
	SoundData soundData1;
	SoundData soundData2;
	SoundData soundData3;

	// プレイヤー
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<KModel> playerModel;

	// プレイヤーの弾モデル
	std::unique_ptr<KModel> playersBulletModel = nullptr;

	// 雑魚
	std::list<std::unique_ptr<MobEnemy>> mobEnemys;
	std::unique_ptr<KModel> mobEnemysModel;

	// 敵の弾モデル
	std::unique_ptr<KModel> enemysBulletModel = nullptr;

	// 簡易地面
	std::unique_ptr<Ground> ground = nullptr;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	bool isStand = false;
	int32_t waitTimer = 0;

	TextureData textureData;
	TextureData textureData2;

	// ボスバトル
	std::unique_ptr<BossEnemy> boss = nullptr;

	// 登場警告
	std::unique_ptr<Warning> bossWarning = nullptr;
	bool isBossBattle = false;

	// バレットマネージャー
	BulletManager* bulletManager = nullptr;

	// スカイボックス
	std::unique_ptr<SkyBox> skyBox = nullptr;
	std::unique_ptr<KModel> skyBoxModel = nullptr;

	// パーティクル
	ParticleManager* particleManager = nullptr;

	// 別のシーンへの移行タイマー
	// ゲームクリア
	float goClearSceneTime = 120;
	float goClearSceneTimer = 0;

	// ゲームオーバー
	float goOverSceneTime = 120;
	float goOverSceneTimer = 0;
};

