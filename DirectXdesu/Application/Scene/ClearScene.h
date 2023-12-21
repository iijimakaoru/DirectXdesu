#pragma once
#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"

/**
 * @file ClearScene.h
 * @brief ゲームクリアシーン
 * @author 飯島 薫
 */

class ClearScene : public BaseScene {
public:
	ClearScene(){};
	~ClearScene();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Final() override;

private:
	// 背景動かし
	void MoveBack();

	void GameScoreDraw();

	void EnemyScoreDraw();

	void MinDamageScoreDraw();

	void BossTimeScoreDraw();

	void TotalScoreDraw();

private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> spritePipeline = nullptr;

	// シェーダー
	KShader spriteShader;

	// クリアスプライト
	std::unique_ptr<Sprite> result = nullptr;
	TextureData resultTex;
	KMyMath::Vector2 resultPos;

	// スコアボード
	std::unique_ptr<Sprite> scoreBord;
	TextureData scoreBordTex;
	KMyMath::Vector2 scoreBordPos;
	KMyMath::Vector2 scoreBordSize;

	// ゲーム内スコア
	std::unique_ptr<Sprite> gameScore;
	TextureData gameScoreTex;
	KMyMath::Vector2 gameScorePos;
	std::array<std::unique_ptr<Sprite>, 6> gameScoreS;
	KMyMath::Vector2 gameScoreSPos;
	size_t gameScoreNum = 0;

	// 敵撃破数スコア
	std::unique_ptr<Sprite> enemyScore;
	TextureData enemyScoreTex;
	KMyMath::Vector2 enemyScorePos;
	std::array<std::unique_ptr<Sprite>, 6> enemyScoreS;
	KMyMath::Vector2 enemyScoreSPos;
	size_t enemyScoreNum = 0;

	// 被弾率スコア
	std::unique_ptr<Sprite> minDamageScore;
	TextureData minDamageScoreTex;
	KMyMath::Vector2 minDamageScorePos;
	std::array<std::unique_ptr<Sprite>, 6> minDamageScoreS;
	KMyMath::Vector2 minDamageScoreSPos;
	size_t minDamageScoreNum = 0;

	// ボス撃破時間スコア
	std::unique_ptr<Sprite> bossTimeScore;
	TextureData bossTimeScoreTex;
	KMyMath::Vector2 bossTimeScorePos;
	std::array<std::unique_ptr<Sprite>, 6> bossTimeScoreS;
	KMyMath::Vector2 bossTimeScoreSPos;
	size_t bossTimeScoreNum = 0;

	// 総合スコア
	std::unique_ptr<Sprite> total;
	TextureData totalTex;
	KMyMath::Vector2 totalPos;
	std::array<std::unique_ptr<Sprite>, 6> totalS;
	KMyMath::Vector2 totalSPos;
	size_t totalScoreNum = 0;

	bool isDrawScores = false;

	// 数字のテキスト
	std::array<TextureData, 10> numTexs;

	// 次のシーンへのキー
	std::unique_ptr<Sprite> pushA = nullptr;
	TextureData pushATex;
	KMyMath::Vector2 pushAPos = {0, 0};

	// スプライト背景
	std::array<std::unique_ptr<Sprite>, 2> back;
	TextureData backTex;
	std::array<KMyMath::Vector2, 2> backPos;

	// リザルトフェーズ
	size_t resultPhase = 0;
	float phaseTimer = 0;
	float phaseTime = 0;

	bool isGoScene = false;
};
