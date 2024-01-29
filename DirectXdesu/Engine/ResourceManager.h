#pragma once
#include <array>
#include "TextureManager.h"

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

private:
	// タイトルスプライト
	TextureData mesiTex;
	TextureData shooterTex;
	TextureData moldTex;

	TextureData pushATex;

	// 数字のテキスト
	std::array<TextureData, 10> numTexs;

	// スコアテキスト
	TextureData scoresTex;

	// 「Hit」テキスト
	TextureData hitsTex;

	// 「x」テキスト
	TextureData xTex;

	// 白1x1テキスト(たくさん使えるテクスチャ)
	TextureData white1x1Tex;

	// ボーナスゲージ
	TextureData bonusGageTex;
};
