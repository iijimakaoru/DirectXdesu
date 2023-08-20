#pragma once
#include "Sprite.h"

class SceneChange
{
public:
	void Init();

	void Update();

	void Draw();

public:
	static SceneChange* GetInstance();

	void Delete();

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

	// アルファ値
	float alpha = 0.0f;

	// 削除フラグ
	bool isChange = false;

private:
	static SceneChange* sceneChange;
};

