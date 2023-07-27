#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"
#include "TextureManager.h"

class Bullet
{
public:
	// 初期化
	virtual void Init() = 0;

	/// <summary>
	/// 呼び出し
	/// </summary>
	/// <param name="pos"></param>
	virtual void Set(const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro);

	// 描画
	virtual void Draw();

	// ゲッター
	virtual bool GetIsDead() const
	{
		return isDead;
	}

	// 衝突時に呼び出し
	virtual void OnCollision();

	// ワールドポジションゲッター
	virtual KMyMath::Vector3 GetWorldPos();

protected:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	std::unique_ptr<KModel> model;

	// テクスチャ
	TextureData texData;

	// パイプライン
	std::unique_ptr<KGPlin> pipeline;

	// シェーダー
	KShader shader;

	// スピード
	float speed = 1.0f;

	bool isDead = false;

	KMyMath::Vector3 vec;

	float lifeTimer = 0.0f;
};