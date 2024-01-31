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
