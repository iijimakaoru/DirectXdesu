#pragma once
#include "Sprite.h"

class Reticle2D
{
public:
	void Init();

	void Update();

	void Draw();

	// ポジションゲッター
	const KMyMath::Vector2 GetPos() const;

protected:
	// スプライト
	std::unique_ptr<Sprite> reticle = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> pipeline;

	// シェーダー
	KShader shader;

	// テクスチャ
	KTexture tex;

	KMyMath::Vector2 position;

	KMyMath::Vector2 size;

	KMyMath::Vector4 color;
};