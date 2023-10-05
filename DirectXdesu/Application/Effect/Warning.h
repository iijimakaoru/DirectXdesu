#pragma once
#include "Sprite.h"

class Warning
{
public:
	void Init();

	void Update();

	void Draw();

	bool GetIsDelete() { return isDelete; }

	int32_t GetTime() { return warningTime; }

private:
	// スプライト
	std::unique_ptr<Sprite> warning = nullptr;

	// テクスチャ
	TextureData warningTex;

	// パイプライン
	std::unique_ptr<KGPlin> pipeline = nullptr;

	// シェーダー
	KShader shader;

	// 警告時間
	int32_t warningTimer = 0;
	int32_t warningTime = 0;

	// アルファ値
	float alpha = 0.0f;

	// アルファ値変更スピード
	float alphaSpeed = -0.03f;

	// 削除フラグ
	bool isDelete = false;
};

