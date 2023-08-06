#pragma once
#include "MobEnemy.h"

class AppearEnemy : public MobEnemy
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model_">使用するモデル</param>
	/// <param name="pos">初期位置</param>
	/// <returns></returns>
	static AppearEnemy* Create(KModel* model_,KGPlin* pipeline_, const KMyMath::Vector3& pos);

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

	// クールタイム
	float coolTimer = 0;
	const float coolTime = 120;
};

