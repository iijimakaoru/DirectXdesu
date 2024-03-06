#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"

/**
 * @file MobEnemy.h
 * @brief 雑魚敵の基底クラス
 * @author 飯島 薫
 */

class Player;

class MobEnemy
{
public:
	enum EnemysType
	{
		None,
		Fly,
		Canon,
		Appear
	};

	MobEnemy();
	virtual ~MobEnemy();

	// 初期化
	virtual void Init(KModel* model_, KGPlin* pipeline_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos);

	// 描画
	virtual void Draw();

	// 衝突時に呼び出し
	virtual void OnCollision();

	// 死亡判定
	const bool GetIsDead()const
	{
		return isDead;
	}

	// 消去
	const bool GetIsDelete() const;

	// 自滅
	void SelfDestruction();

	/// <summary>
	/// 攻撃
	/// </summary>
	virtual void Attack();

	// ワールドポジションゲッター
	virtual const KMyMath::Vector3 GetWorldPos() const;

	// プレイヤーポインタセッター
	virtual void SetPlayer(Player* player_) { player = player_; }

protected:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// 死亡フラグ
	bool isDead = false;

	// 消去
	bool isDelete = false;

	// クールタイム
	float coolTimer = 0;
	float coolTime = 120;

	// 自キャラ
	Player* player = nullptr;

private:
	// モデル
	KModel* model = nullptr;

	// パイプライン
	KGPlin* pipeline;
};