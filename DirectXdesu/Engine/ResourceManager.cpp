#include "ResourceManager.h"

ResourceManager* ResourceManager::GetInstance() {
	static ResourceManager instance;

	return &instance;
}

void ResourceManager::Init() {
	// タイトル名テクスチャ
	mesiTex = TextureManager::Load("MESI", ".png");
	shooterTex = TextureManager::Load("SHOOTER", ".png");
	moldTex = TextureManager::Load("BattleShipMold", ".png");

	// プッシュAテクスチャ
	pushATex = TextureManager::Load("kariNextScene", ".png");

	// 数字読み込み
	numTexs[0] = TextureManager::Load("Num0", ".png");
	numTexs[1] = TextureManager::Load("Num1", ".png");
	numTexs[2] = TextureManager::Load("Num2", ".png");
	numTexs[3] = TextureManager::Load("Num3", ".png");
	numTexs[4] = TextureManager::Load("Num4", ".png");
	numTexs[5] = TextureManager::Load("Num5", ".png");
	numTexs[6] = TextureManager::Load("Num6", ".png");
	numTexs[7] = TextureManager::Load("Num7", ".png");
	numTexs[8] = TextureManager::Load("Num8", ".png");
	numTexs[9] = TextureManager::Load("Num9", ".png");

	// 「Score」文字
	scoresTex = TextureManager::Load("ScoreTex", ".png");

	// 「HIT」文字
	hitsTex = TextureManager::Load("HitTex", ".png");

	// 「X」文字
	xTex = TextureManager::Load("XTex", ".png");

	// ボーナス時間バースプライト
	white1x1Tex = TextureManager::Load("scoreBonusBar", ".png");

	bonusGageTex = TextureManager::Load("white1x1", ".png");
}

const TextureData& ResourceManager::GetMesiTex() const { return mesiTex; }

const TextureData& ResourceManager::GetShooterTex() const { return shooterTex; }

const TextureData& ResourceManager::GetMoldTex() const { return moldTex; }

const TextureData& ResourceManager::GetPushATex() const { return pushATex; }

const TextureData& ResourceManager::GetNumeTexs(size_t i) const { return numTexs[i]; }

const TextureData& ResourceManager::GetScoresTex() const { return scoresTex; }

const TextureData& ResourceManager::GetHitsTex() const { return hitsTex; }

const TextureData& ResourceManager::GetXsTex() const { return xTex; }

const TextureData& ResourceManager::GetWhite1x1Tex() const { return white1x1Tex; }

const TextureData& ResourceManager::GetBonusGageTex() const { return bonusGageTex; }
