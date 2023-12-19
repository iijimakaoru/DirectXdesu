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

	void AddMobScore(size_t score_);
	void AddBossScore(size_t score_);

private:
	// スコアの桁分のスプライト
	std::array<std::unique_ptr<Sprite>, 6> nums;
	// スコアの場所
	KMyMath::Vector2 numsPos = {200, 42};
	// 数字のテキスト
	std::array<TextureData, 10> numTexs;

	// 「Score」テクスチャ用
	std::unique_ptr<Sprite> scores;
	KMyMath::Vector2 scoresPos = {30, 30};
	TextureData scoresTex;

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
