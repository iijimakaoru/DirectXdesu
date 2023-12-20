#pragma once
#include "Sprite.h"

class ScoreManager {
public:
	// インスタンス
	static ScoreManager* GetInstance();

private:
	ScoreManager() = default;
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	const ScoreManager& operator=(const ScoreManager&) = delete;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// スコアリセット
	/// </summary>
	void ResetScore();

	void AddMobScore(size_t score_);
	void AddBossScore(size_t score_);

	const size_t& GetGameScore() const;

private:
	/// <summary>
	/// ボーナス中
	/// </summary>
	void CountBonusTimer();

private:
	// スコアの桁分のスプライト
	std::array<std::unique_ptr<Sprite>, 6> score;
	// スコアの場所
	KMyMath::Vector2 numsPos = {200, 42};
	// 数字のテキスト
	std::array<TextureData, 10> numTexs;

	// 加算スコアのスプライト
	std::array<std::unique_ptr<Sprite>, 4> addScore;

	// ボーナスカウントのスプライト
	std::array<std::unique_ptr<Sprite>, 2> bonusCountS;

	//「Score」テクスチャ用
	std::unique_ptr<Sprite> scores;
	KMyMath::Vector2 scoresPos = {30, 30};
	TextureData scoresTex;

	//「HIT」テクスチャ用
	std::unique_ptr<Sprite> hits;
	KMyMath::Vector2 hitsPos = {30, 30};
	TextureData hitsTex;

	//「X」テクスチャ用
	std::unique_ptr<Sprite> xS;
	KMyMath::Vector2 xPos = {30, 30};
	TextureData xTex;

	// ボーナスカウントバー
	std::unique_ptr<Sprite> bonusBar;
	TextureData bonusBarTex;
	std::unique_ptr<Sprite> bonusGage;
	TextureData bonusGageTex;

	std::unique_ptr<KGPlin> spritePipeline;
	KShader spriteShader;

	/// <summary>
	/// スコアのやつ(ゴリ押し)
	/// </summary>
	size_t gameScore = 0;
	size_t oldGameScore = 0;

	/// <summary>
	/// 加算するスコア
	/// </summary>
	size_t addScoreNum = 0;
	size_t addResultScore = 0;
	bool isCount = false;

	/// <summary>
	/// ボーナスの受付時間
	/// </summary>
	bool isBonus = false;
	float bonusTimer = 0;
	const float bonusTime = 180;
	size_t bonusCount = 0;

	/// <summary>
	/// 加算時間
	/// </summary>
	bool isAddScore = false;
	float addScoreTimer = 0;
	const float addScoreTime = 60;
};
