#pragma once
#include "MobEnemy.h"
class FlyEnemy : public MobEnemy
{
public:
	static FlyEnemy* Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos_, const KMyMath::Vector2& endPos_, const float& speed_);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	void Init(KModel* model_, KGPlin* pipeline_) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	/// <param name="cameraPos"></param>
	void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack() override;

	/// <summary>
	/// 出現
	/// </summary>
	void Appear();

private:
	/// <summary>
	/// スタートポジション
	/// </summary>
	/// <param name="startPos_"></param>
	void SetStartPos(const KMyMath::Vector3& startPos_);

	/// <summary>
	/// エンドポジション
	/// </summary>
	/// <param name="endPos_"></param>
	void SetEndPos(const KMyMath::Vector2& endPos_);

	/// <summary>
	/// スピードセット
	/// </summary>
	/// <param name="speed"></param>
	void SetSpeed(const float& speed);

private:
	// イージングタイマー
	float easeTimer = 0;

	// 上限
	const float easeTime = 30;

	// 最初のスケール
	KMyMath::Vector3 startPos;

	// 最終スケール
	KMyMath::Vector2 endPos;

	// 出現演出中かどうか
	bool isAppear = false;

	// スピード
	float moveSpeed = 0;
};

