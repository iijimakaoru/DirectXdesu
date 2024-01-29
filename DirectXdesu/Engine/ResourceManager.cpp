#include "ResourceManager.h"

ResourceManager* ResourceManager::GetInstance() {
	static ResourceManager instance;

	return &instance;
}

void ResourceManager::Init() {
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
	bonusBarTex = TextureManager::Load("Resources/texture/scoreBonusBar.png");

	// ボーナス時間ゲージスプライト
	bonusGageTex = TextureManager::Load("Resources/texture/white1x1.png");
}

const TextureData& ResourceManager::GetNumeTexs(size_t i) const { return numTexs[i]; }

const TextureData& ResourceManager::GetScoresTex() const { return scoresTex; }

const TextureData& ResourceManager::GetHitsTex() const { return hitsTex; }

const TextureData& ResourceManager::GetXsTex() const { return xTex; }

const TextureData& ResourceManager::GetBonusBarTex() const { return bonusBarTex; }

const TextureData& ResourceManager::GetBonusGageTex() const { return bonusGageTex; }
