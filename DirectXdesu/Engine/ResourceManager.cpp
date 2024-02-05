#include "ResourceManager.h"

ResourceManager* ResourceManager::GetInstance() {
	static ResourceManager instance;

	return &instance;
}

void ResourceManager::Init() {
	// タイトル名テクスチャ
	mesiTex = TextureManager::Load("Resources/texture/MESI.png");
	shooterTex = TextureManager::Load("Resources/texture/SHOOTER.png");
	moldTex = TextureManager::Load("Resources/texture/BattleShipMold.png");

	// ポーズ
	poseTexT = TextureManager::Load("Resources/texture/pose.png");
	backTitleT = TextureManager::Load("Resources/texture/BackTitle.png");
	operationT = TextureManager::Load("Resources/texture/Operation.png");

	// クリア
	// リザルトテクスチャ
	resultTex = TextureManager::Load("Resources/texture/ResultText.png");
	// スコアボードテクスチャ
	scoreBordTex = TextureManager::Load("Resources/texture/ScoreBord.png");
	// 背景テクスチャ
	backTex = TextureManager::Load("Resources/texture/ResultBack.png");
	// スコア
	gameScoreTex = TextureManager::Load("Resources/texture/LevelScore.png");
	enemyScoreTex = TextureManager::Load("Resources/texture/EnemyBonus.png");
	minDamageScoreTex = TextureManager::Load("Resources/texture/MinDamageBonus.png");
	bossTimeScoreTex = TextureManager::Load("Resources/texture/BossTimeBonus.png");
	totalTex = TextureManager::Load("Resources/texture/Total.png");

	// プッシュAテクスチャ
	pushATex = TextureManager::Load("Resources/texture/kariNextScene.png");

	// 数字読み込み
	numTexs[0] = TextureManager::Load("Resources/texture/Num0.png");
	numTexs[1] = TextureManager::Load("Resources/texture/Num1.png");
	numTexs[2] = TextureManager::Load("Resources/texture/Num2.png");
	numTexs[3] = TextureManager::Load("Resources/texture/Num3.png");
	numTexs[4] = TextureManager::Load("Resources/texture/Num4.png");
	numTexs[5] = TextureManager::Load("Resources/texture/Num5.png");
	numTexs[6] = TextureManager::Load("Resources/texture/Num6.png");
	numTexs[7] = TextureManager::Load("Resources/texture/Num7.png");
	numTexs[8] = TextureManager::Load("Resources/texture/Num8.png");
	numTexs[9] = TextureManager::Load("Resources/texture/Num9.png");

	// 「Score」文字
	scoresTex = TextureManager::Load("Resources/texture/ScoreTex.png");

	// 「HIT」文字
	hitsTex = TextureManager::Load("Resources/texture/HitTex.png");

	// 「X」文字
	xTex = TextureManager::Load("Resources/texture/XTex.png");

	// ボーナス時間バースプライト
	scoreBonusBar = TextureManager::Load("Resources/texture/scoreBonusBar.png");

	white1x1sTex = TextureManager::Load("Resources/texture/white1x1.png");
}

const TextureData& ResourceManager::GetMesiTex() const { return mesiTex; }

const TextureData& ResourceManager::GetShooterTex() const { return shooterTex; }

const TextureData& ResourceManager::GetMoldTex() const { return moldTex; }

const TextureData& ResourceManager::GetPushATex() const { return pushATex; }

const TextureData& ResourceManager::GetNumeTexs(size_t i) const { return numTexs[i]; }

const TextureData& ResourceManager::GetScoresTex() const { return scoresTex; }

const TextureData& ResourceManager::GetHitsTex() const { return hitsTex; }

const TextureData& ResourceManager::GetXsTex() const { return xTex; }

const TextureData& ResourceManager::GetWhite1x1Tex() const { return white1x1sTex; }

const TextureData& ResourceManager::GetBonusGageTex() const { return scoreBonusBar; }

const TextureData& ResourceManager::GetPoseTex() const { return poseTexT; }

const TextureData& ResourceManager::GetBackTitleTex() const { return backTitleT; }

const TextureData& ResourceManager::GetOperationTex() const { return operationT; }

const TextureData& ResourceManager::GetResultTex() const { return resultTex; }

const TextureData& ResourceManager::GetScoreBordTex() const { return scoreBordTex; }

const TextureData& ResourceManager::GetGameScoreTex() const { return gameScoreTex; }

const TextureData& ResourceManager::GetEnemyScoreTex() const { return enemyScoreTex; }

const TextureData& ResourceManager::GetMinDamageScoreTex() const { return minDamageScoreTex; }

const TextureData& ResourceManager::GetbossTimeScoreTex() const { return bossTimeScoreTex; }

const TextureData& ResourceManager::GetTotalScoreTex() const { return totalTex; }

const TextureData& ResourceManager::GetBackTex() const { return backTex; }
