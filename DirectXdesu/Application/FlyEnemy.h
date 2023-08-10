#pragma once
#include "MobEnemy.h"
class FlyEnemy : public MobEnemy
{
public:
	static FlyEnemy* Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos, const KMyMath::Vector2& endPos);

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
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 出現
	/// </summary>
	void Appear();

private:
	// イージングタイマー
	float easeTimer = 0;

	// 上限
	const float easeTime = 30;

	// 最初のスケール
	KMyMath::Vector3 startScale;

	// 最終スケール
	KMyMath::Vector3 endScale;

	// 出現演出中かどうか
	bool isAppear = false;
};

