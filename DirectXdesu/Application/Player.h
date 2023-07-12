#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"
#include "KInput.h"
#include "Bullet.h"

#include "Reticle3D.h"
#include "Reticle2D.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	// 初期化
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera_"></param>
	void Update(ViewProjection* viewPro);

	// 移動
	void Move();

	// 回転
	void Rot();

	// 攻撃
	void Attack();

	// 描画
	void Draw();

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransfom* parent);

	// ポジションゲッター
	KMyMath::Vector3& GetPosition()
	{
		return object3d->transform.pos;
	}

	// ワールドポジションゲッター
	KMyMath::Vector3 GetWorldPos();

	// 衝突時に呼び出し
	void OnCollision();

	// 弾のリストを取得
	const std::list<std::unique_ptr<Bullet>>& GetBullets() 
	{ 
		return bullets; 
	}

private:
	// Input
	KInput* input = nullptr;
	// パッドの入力情報
	KMyMath::Vector2 leftStickPos;

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

	// 回転戻すイージング用
	KMyMath::Vector2 oldRot;
	float rotEaseTimer = 0;
	bool isRotEase = false;

	// Z軸
	bool isRotZRight = false;
	float swayZ = 0.0f;

	// 動くスピード
	float moveSpeed = 0.16f;

	// 角度上限
	const KMyMath::Vector2 rotLimit = { 35.0f, 25.0f };

	// 弾
	std::list<std::unique_ptr<Bullet>> bullets;

	// レティクル
	std::unique_ptr<Reticle3D> reticle3d = nullptr;
	std::unique_ptr<Reticle2D> reticle2d = nullptr;
};