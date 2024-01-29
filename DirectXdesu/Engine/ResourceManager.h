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

	const TextureData& GetNumeTexs(size_t i) const;

	const TextureData& GetScoresTex() const;

	const TextureData& GetHitsTex() const;

	const TextureData& GetXsTex() const;

	const TextureData& GetBonusBarTex() const;

	const TextureData& GetBonusGageTex() const;

private:
	// 数字のテキスト
	std::array<TextureData, 10> numTexs;

	// スコアテキスト
	TextureData scoresTex;

	// 「Hit」テキスト
	TextureData hitsTex;

	// 「x」テキスト
	TextureData xTex;

	// ボーナスバー
	TextureData bonusBarTex;

	// ボーナスゲージ
	TextureData bonusGageTex;
};
