#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "ViewProjection.h"
#include "TextureManager.h"

class BossEnemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="pipeline_"></param>
	/// <param name="initPos"></param>
	/// <param name="HP"></param>
	virtual void Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos, const float HP) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ワールド座標ゲッター
	/// </summary>
	/// <returns></returns>
	virtual const KMyMath::Vector3 GetWorldPos() const;

	// 当たった時の処理
	virtual void OnCollision();

protected:
	// トランスフォーム
	std::unique_ptr<KObject3d> object3d = nullptr;

	// モデル
	KModel* model = nullptr;

	// パイプライン
	KGPlin* pipeline = nullptr;

	// 体力
	float HP = 0;
};

