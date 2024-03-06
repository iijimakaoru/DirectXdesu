#pragma once
#include "Sprite.h"
#include "Camera.h"

/**
 * @file Reticle2D.h
 * @brief 2Dレティクル
 * @author 飯島 薫
 */

class Reticle2D
{
public:
	// 初期化
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	/// <param name="reticle3dPos"></param>
	void Update(ViewProjection* viewPro, KMyMath::Vector3 reticle3dPos);

	// 描画
	void Draw();

	// ポジションゲッター
	const KMyMath::Vector2 GetPos() const;

protected:
	// スプライト
	std::unique_ptr<Sprite> reticle = nullptr;

	// テクスチャ
	TextureData texData;

	KMyMath::Vector2 position;

	KMyMath::Vector2 scale;

	KMyMath::Vector4 color;

	float rot;
};