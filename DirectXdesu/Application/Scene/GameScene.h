#pragma once
#include "BaseScene.h"
#include "BillManager.h"
#include "Blaster.h"
#include "BossEnemy.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "FbxObject3D.h"
#include "Ground.h"
#include "KDirectXCommon.h"
#include "KGPlin.h"
#include "KInput.h"
#include "KModel.h"
#include "KObject3d.h"
#include "KShader.h"
#include "MobEnemy.h"
#include "ParticleManager.h"
#include "Player.h"
#include "PostEffect.h"
#include "RailCamera.h"
#include "SkyBox.h"
#include "Sprite.h"
#include "Warning.h"
#include <array>
#include <imgui.h>

#include "AudioManager.h"

#include "Light.h"

#include "GameManager.h"
#include "Movie.h"
#include "CollisionManager.h"

/**
 * @file GameScene.h
 * @brief ゲームシーン
 * @author 飯島 薫
 */

class GameScene : public BaseScene {
public:
	enum Scene {
		Games = 0,
		Over = 1,
		Movies = 2
	};

public:
	GameScene(){};
	~GameScene();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Final() override;

private:
	// 衝突判定
	void CheckAllCollisions();

	// ボスバトル開始
	void BossBattleStart();

	// 自機死亡
	void PlayerDead();

	// ゲームオーバーシーンへ
	void GoGameOverScene();

	// ボス撃破
	void BossBreakMovie();

	// ゲームプレイ中
	void GamePlay();

	// 全シーン共通
	void AllScene();

	void PoseAction();

public:
	// フラグゲッター
	const bool GetIsStart() const;

private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<RailCamera> camera = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player = nullptr;

	// 敵の弾モデル
	std::unique_ptr<KModel> enemysBulletModel = nullptr;

	// 簡易地面
	std::unique_ptr<Ground> ground = nullptr;

	// ボスバトル
	int32_t bossBattleTimer = 0;
	int32_t bossBattleTime = 0;

	// MS01_Blaster
	std::unique_ptr<Blaster> blaster = nullptr;

	// 登場警告
	std::unique_ptr<Warning> bossWarning = nullptr;
	bool isBossBattle = false;

	// バレットマネージャー
	BulletManager* bulletManager = nullptr;

	// スカイボックス
	std::unique_ptr<SkyBox> skyBox = nullptr;

	// パーティクル
	ParticleManager* particleManager = nullptr;
	ObjParticleManager* objParticleManager = nullptr;

	// クリアムービーへの移行タイマー
	float goClearMovieTime = 60.0f;
	float goClearMovieTimer = 0;

	// ゲームオーバー
	bool isGoOverScene = false;
	float goOverSceneTime = 60.0f;
	float goOverSceneTimer = 0;

	// ビルマネージャー
	std::unique_ptr<BillManager> billManager;

	// 死亡カメラ呼び出しフラグ
	bool isCallDeadCamera = false;

#pragma region ステージスタートムービー
	// フラグ
	bool isStageStart = false;
#pragma endregion

#pragma region ボス出現ムービー
	// ボス出現ムービーフラグ
	bool isBossAppearMovie = false;
#pragma endregion

	// 警告演出フラグ
	bool isWarnning = false;

	// エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyManager = nullptr;

	// ボスバトル開始座標
	float bossBattleStartPos = 1000;

#pragma region クリア演出
	// クリアムービーフラグ
	bool isClearMovie = false;
#pragma endregion

#pragma region オーバー演出
	// ゲームオーバームービーフラグ
	bool isOverMovie = false;
#pragma endregion

#pragma region ポーズ
	bool isPose = false;

	bool isOperation = true;
	bool isBackTitle = false;

	std::unique_ptr<Sprite> poseBack;

	std::unique_ptr<Sprite> selectBar;
	KMyMath::Vector2 selectBarPos;

	std::unique_ptr<Sprite> poseTexS;
	KMyMath::Vector2 poseTexPos;

	std::unique_ptr<Sprite> backTitleS;
	KMyMath::Vector2 backTitlePos;

	std::unique_ptr<Sprite> operationS;
	KMyMath::Vector2 operationPos;
#pragma endregion

	AudioManager* audioManager_;

	std::unique_ptr<Light> light_ = nullptr;

	KMyMath::Vector3 lightRGB = {1, 1, 1};
	KMyMath::Vector3 lightDir = {0, -1, 0};

	Scene scene = Scene::Movies;

	std::unique_ptr<BaseMovie> movie_ = nullptr;

	GameManager* gameManager_ = nullptr;

	CollisionManager* collisionManager_ = nullptr;
};
