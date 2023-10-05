#pragma once
#include "Sprite.h"
#include "ViewProjection.h"

class Reticle2D
{
public:
	void Init();

	void Update(ViewProjection* viewPro,KMyMath::Vector3 reticle3dPos);

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
	TextureData texData;

	KMyMath::Vector2 position;

	KMyMath::Vector2 scale;

	KMyMath::Vector4 color;

	float rot;
};