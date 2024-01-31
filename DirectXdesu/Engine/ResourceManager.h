#pragma once
#include "TextureManager.h"
#include <array>

class ResourceManager {
public:
	static ResourceManager* GetInstance();

private:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	const ResourceManager& operator=(const ResourceManager&) = delete;

public:
	void Init();

	// 「Mesi」
	const TextureData& GetMesiTex() const;
	// 「Shooter」
	const TextureData& GetShooterTex() const;
	// タイトルバック
	const TextureData& GetMoldTex() const;

	// 「Aを押してください」
	const TextureData& GetPushATex() const;

	// 数字
	const TextureData& GetNumeTexs(size_t i) const;
	// 「Score」
	const TextureData& GetScoresTex() const;
	// 「Hit」
	const TextureData& GetHitsTex() const;
	// 「x」
	const TextureData& GetXsTex() const;
	// 白1x1テクスチャ
	const TextureData& GetWhite1x1Tex() const;
	// ボーナスゲージ
	const TextureData& GetBonusGageTex() const;

	// 「ポーズ」
	const TextureData& GetPoseTex() const;
	// 「タイトルへ」
	const TextureData& GetBackTitleTex() const;
	// 「操作説明」
	const TextureData& GetOperationTex() const;

private:
#pragma region タイトル
	TextureData mesiTex;
	TextureData shooterTex;
	TextureData moldTex;
#pragma endregion

#pragma region ゲーム
	// ポーズ
	TextureData poseTexT;
	TextureData backTitleT;
	TextureData operationT;
#pragma endregion

#pragma region スコア
	// スコアテキスト
	TextureData scoresTex;
	// 「Hit」テキスト
	TextureData hitsTex;
	// 「x」テキスト
	TextureData xTex;
	// ボーナスゲージ
	TextureData scoreBonusBar;
#pragma endregion

	// プッシュA
	TextureData pushATex;

	// 数字のテキスト
	std::array<TextureData, 10> numTexs;

	// 白1x1テキスト(たくさん使えるテクスチャ)
	TextureData white1x1sTex;
};
