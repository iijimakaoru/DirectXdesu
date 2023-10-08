#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"

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
	~MobEnemy();

	// 初期化
	virtual void Init(KModel* model_, KGPlin* pipeline_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos_);

	// 描画
	virtual void Draw();

	// 衝突時に呼び出し
	virtual void OnCollision();

	// 死亡判定
	virtual const bool GetIsDead()const
	{
		return isDead;
	}

	void SetCameraPos(const KMyMath::Vector3& cameraPos_);

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

	// クールタイム
	float coolTimer = 0;
	float coolTime = 120;

	// 自キャラ
	Player* player = nullptr;

	KMyMath::Vector3 cameraPos;

private:
	// モデル
	KModel* model = nullptr;

	// パイプライン
	KGPlin* pipeline;
};