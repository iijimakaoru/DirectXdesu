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

	// 回転
	void SetRot(const float& rot);

	// スケール
	void SetScale(const KMyMath::Vector2& scale);

	// 色
	void SetColor(const KMyMath::Vector3& rgb_);

protected:
	// スプライト
	std::unique_ptr<Sprite> reticle = nullptr;

	// テクスチャ
	TextureData texData;

	KMyMath::Vector2 position;

	KMyMath::Vector2 scale_;

	KMyMath::Vector4 color;

	float rot_;
};