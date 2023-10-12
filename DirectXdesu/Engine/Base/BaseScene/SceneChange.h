#pragma once
#include "Sprite.h"

/**
 * @file SceneChange.h
 * @brief シーンチェンジ演出
 * @author 飯島 薫
 */

class SceneChange
{
public:
	void Init();

	void Update();

	void Draw();

	void Start();

	void End();

public:
	static SceneChange* GetInstance();

	void Delete();

public:
	const bool GetIsChange() const;

	const bool GetIsEffect() const;

private:
	SceneChange() = default;
	~SceneChange() = default;
	SceneChange(const SceneChange&) = delete;
	const SceneChange& operator=(const SceneChange&) = delete;

private:
	// スプライト
	std::unique_ptr<Sprite> black = nullptr;

	// テクスチャ
	TextureData blackTex;

	// パイプライン
	std::unique_ptr<KGPlin> pipeline = nullptr;

	// シェーダー
	KShader shader;

	// 暗転時間
	int32_t inTimer = 0;
	int32_t inTime = 30;
	bool isIn = false;

	// 解除時間
	int32_t outTimer = 0;
	int32_t outTime = 30;
	bool isOut = false;

	bool isStart = false;

	bool isEffect = false;

	// アルファ値
	float alpha = 0.0f;

	// 削除フラグ
	bool isChange = false;

private:
	static SceneChange* sceneChange;
};

