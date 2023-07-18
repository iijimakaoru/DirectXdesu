#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"

class MobEnemy
{
public:
	// 初期化
	void Init(KModel* model_);

	// 仮配置
	void Set(KMyMath::Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro,const KMyMath::Vector3& cameraPos);

	// 描画
	void Draw();

	// 衝突時に呼び出し
	void OnCollision();

	const bool GetIsDead()const
	{
		return isDead;
	}

	// ワールドポジションゲッター
	KMyMath::Vector3 GetWorldPos();

private:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	KModel* model = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> pipeline;

	// シェーダー
	KShader shader;

	// スピード
	float speed = 0.5f;

	bool isDead = false;
};