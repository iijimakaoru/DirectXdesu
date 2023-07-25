#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"

class MobEnemy
{
public:
	enum EnemysKind
	{
		None,
		Battery,
		Fall,
		Appear
	};

	// 初期化
	virtual void Init(KModel* model_, KGPlin* pipeline_) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro,const KMyMath::Vector3& cameraPos) = 0;

	// 描画
	virtual void Draw();

	// 衝突時に呼び出し
	virtual void OnCollision();

	virtual const bool GetIsDead()const
	{
		return isDead;
	}

	// ワールドポジションゲッター
	virtual const KMyMath::Vector3 GetWorldPos() const;

protected:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	KModel* model = nullptr;

	// 死亡フラグ
	bool isDead = false;

	// パイプライン
	KGPlin* pipeline;

	// スピード
	float speed = 0.5f;
};