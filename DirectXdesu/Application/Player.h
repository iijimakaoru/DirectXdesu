#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"
#include "KInput.h"
#include "PlayerBullet.h"

#include "Reticle3D.h"
#include "Reticle2D.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	// 初期化
	void Init(KModel* model_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera_"></param>
	void Update(ViewProjection* viewPro);

	// 描画
	void Draw();

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransfom* parent);

	// ポジションゲッター
	const KMyMath::Vector3& GetPosition() const
	{
		return object3d->transform.pos;
	}

	// ワールドポジションゲッター
	KMyMath::Vector3 GetWorldPos();

	// 回転ゲッター
	const KMyMath::Vector3 GetRot() const
	{
		return object3d->transform.rot;
	}

	// 衝突時に呼び出し
	void OnCollision();

	// 弾のリストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() 
	{ 
		return bullets; 
	}

	// ゲッター
	static const KMyMath::Vector2& GetRotLimit() { return rotLimit; }
	static const float GetMoveSpeed() { return moveSpeed; }
	static const KMyMath::Vector2& GetPosLimitMax() { return posLimitMax; }
	static const KMyMath::Vector2& GetPosLimitMin() { return posLimitMin; }
private:
	// 移動
	void Move();

	// 回転
	void Rot();

	// 攻撃
	void Attack();

private:
	// Input
	KInput* input = nullptr;
	// パッドの入力情報
	KMyMath::Vector2 leftStickPos;

	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	KModel* model = nullptr;

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

	// 弾
	std::list<std::unique_ptr<PlayerBullet>> bullets;

	// レティクル
	std::unique_ptr<Reticle3D> reticle3d = nullptr;
	std::unique_ptr<Reticle2D> reticle2d = nullptr;

	// 動くスピード
	static const float moveSpeed; // = 0.16f

	// 角度上限
	static const KMyMath::Vector2 rotLimit; // = { 35.0f, 25.0f };

	// 移動上限
	static const KMyMath::Vector2 posLimitMax; // = { 45.0f, 25.0f };
	static const KMyMath::Vector2 posLimitMin; // = { 45.0f, 25.0f };
};