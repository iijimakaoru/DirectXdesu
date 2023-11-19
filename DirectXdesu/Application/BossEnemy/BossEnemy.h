#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Sprite.h"

/**
 * @file BossEnemy.h
 * @brief ボスの基底クラス
 * @author 飯島 薫
 */

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
		const float HP_, KGPlin* spritePipeline_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro, bool isBossMovie_);

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
	virtual bool CollisionCheck(const KMyMath::Vector3& posA, const KMyMath::Vector3& posB);

	// 死亡演出
	virtual void DeadEffect();

	// HP演出
	void HPEffect();

	// HPゲッター
	const bool GetIsHPE() const;

	// 死亡ゲッター
	const bool GetIsDead() const;

	// 墜落エフェクト終わりゲッター
	const bool GetIsFallEffectEnd() const;

	// ゲッター
	const KMyMath::Vector3& GetRot() const;

	// セッター
	void SetPos(const KMyMath::Vector3& pos_);
	void SetRot(const KMyMath::Vector3& rot_);

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

	// ボスムービーフラグ
	bool isBossMovie = false;

#pragma region HP
	// 最大体力
	float maxHP;

	// 現体力
	float HP;
	float oldHP;
	float startHpEase;

	uint32_t oldHpTimer = 0;
	const uint32_t oldHpTime = 60;

	bool hpEase = false;
	float hpEaseTimer = 0;
	const float hpEaseTime = 15;

	// HP
	std::unique_ptr<Sprite> HPUI = nullptr;
	TextureData hpTex;
	std::unique_ptr<Sprite> HPrectUI = nullptr;
	KMyMath::Vector2 HPUIPos = { 0.0f,0.0f };

	// HPバー
	std::unique_ptr<Sprite> HPBarUI = nullptr;
	TextureData hpbarTex;
	KMyMath::Vector2 HPBarUIPos = { 0.0f,0.0f };

	uint32_t easeTimer = 0;
	bool isHPE = false;
#pragma endregion

#pragma region 撃破演出用
	// 演出時間
	float fallEffectTime = 180;
	float fallEffectTimer = 0;

	// 爆発クールタイム
	float expTime = 5;
	float expTimer = 0;

	bool isFallEffectEnd = false;
#pragma endregion

};

