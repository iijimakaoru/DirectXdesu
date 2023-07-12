#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"

class Bullet
{
public:
	// 初期化
	void Init();

	/// <summary>
	/// 呼び出し
	/// </summary>
	/// <param name="pos"></param>
	void Set(const KMyMath::Vector3 pos, const KMyMath::Vector3 vec_,
		const KMyMath::Vector3 rot_ , const float bulletSpeed);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro);

	// 描画
	void Draw();

	// ゲッター
	bool GetIsDead() const
	{
		return isDead;
	}

	// 衝突時に呼び出し
	void OnCollision();

	// ワールドポジションゲッター
	KMyMath::Vector3 GetWorldPos();

private:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	std::unique_ptr<KModel> model;

	// テクスチャ
	KTexture tex;

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