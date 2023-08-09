#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Sprite.h"

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
	virtual void Init(KModel* model_, KGPlin* pipeline_,const KMyMath::Vector3& initPos,
		const float HP, KGPlin* spritePipeline_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro);

	/// <summary>
	/// 
	/// </summary>
	void HPGauge();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();
	virtual void UIDraw();

	/// <summary>
	/// ワールド座標ゲッター
	/// </summary>
	/// <returns></returns>
	virtual const KMyMath::Vector3 GetWorldPos() const;

	// 当たった時の処理
	virtual void OnCollision();

	// 当たり判定範囲
	virtual bool CollisionCheck(const KMyMath::Vector3& posA, const KMyMath::Vector3& posB) = 0;

	const bool GetIsHPE() const;

	const bool GetIsDead() const;

protected:
	// トランスフォーム
	std::unique_ptr<KObject3d> object3d = nullptr;

	// モデル
	KModel* model = nullptr;

	// パイプライン
	KGPlin* pipeline = nullptr;
	KGPlin* spritePipeline = nullptr;

	// 死んでるか
	bool isDead = false;

#pragma region HP
	// 最大体力
	float maxHP;

	// 現体力
	float HP;

	// HP
	std::unique_ptr<Sprite> HPUI = nullptr;
	TextureData hpTex;

	// HPバー
	std::unique_ptr<Sprite> HPBarUI = nullptr;
	TextureData hpbarTex;

	float easeTimer = 0;
	bool isHPE = false;
#pragma endregion
};

