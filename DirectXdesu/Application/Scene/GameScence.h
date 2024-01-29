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

/**
 * @file GameScene.h
 * @brief ゲームシーン
 * @author 飯島 薫
 */

class GameScence : public BaseScene {
public:
	GameScence(){};
	~GameScence();
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

	// スタート演出のムービー
	void StageStartMovie();

	// ゲームオーバーシーンへ
	void GoGameOverScene();

	// ボス出現演出
	void BossAppearMovie();

	// ボス撃破
	void BossBreakMovie();

	// クリアムービー
	void ClearMovie();

	// ムービーバー初期化
	void MovieBarInInit();
	void MovieBarOutInit();

	// ムービーバーにょっき
	void MovieBarOut(const float timer_);

	// ムービーバーにょっき
	void MovieBarIn(const float timer_);

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
	std::unique_ptr<KModel> playerModel;

	// プレイヤーの弾モデル
	std::unique_ptr<KModel> playersBulletModel = nullptr;

	// 雑魚モデル
	std::unique_ptr<KModel> mobEnemysModel;

	// 敵の弾モデル
	std::unique_ptr<KModel> enemysBulletModel = nullptr;

	// 簡易地面
	std::unique_ptr<Ground> ground = nullptr;

	TextureData textureData;
	TextureData textureData2;

	// ボスバトル

	// MS01_Blaster
	std::unique_ptr<Blaster> blaster = nullptr;

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

	// フェーズ
	uint32_t startPhase = 0;

	float startPhaseTimer = 0;
	float startPhaseTime = 0;
#pragma endregion

#pragma region ボス出現ムービー
	// ボス出現ムービーフラグ
	bool isBossAppearMovie = false;

	// フェーズ
	uint32_t appearPhase = 0;

	float appearPhaseTimer = 0;
	float appearPhaseTime = 0;

	// 暗転待ち時間
	float bWaitTimer = 0;
	float bWaitTime = 30;
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

	uint32_t clearPhase = 0;

	float clearPhaseTime = 0;
	float clearPhaseTimer = 0;

	KMyMath::Vector3 start;
	KMyMath::Vector3 p1;
	KMyMath::Vector3 p2;
	KMyMath::Vector3 end;
#pragma endregion

#pragma region オーバー演出
	// ゲームオーバームービーフラグ
	bool isOverMovie = false;
#pragma endregion

#pragma region ムービー中
	//
	std::array<std::unique_ptr<Sprite>, 2> movieBar;
	TextureData movieBarTex;
	std::array<KMyMath::Vector2, 2> movieBarPos;
#pragma endregion

#pragma region ポーズ
	bool isPose = false;

	bool isOperation = true;
	bool isBackTitle = false;

	std::unique_ptr<Sprite> poseBack;

	std::unique_ptr<Sprite> selectBar;
	KMyMath::Vector2 selectBarPos;

	std::unique_ptr<Sprite> poseTexS;
	TextureData poseTexT;
	KMyMath::Vector2 poseTexPos;

	std::unique_ptr<Sprite> backTitleS;
	TextureData backTitleT;
	KMyMath::Vector2 backTitlePos;

	std::unique_ptr<Sprite> operationS;
	TextureData operationT;
	KMyMath::Vector2 operationPos;
#pragma endregion

	AudioManager* audioManager;
};
