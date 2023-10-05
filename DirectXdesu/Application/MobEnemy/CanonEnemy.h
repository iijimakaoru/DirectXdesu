#pragma once
#include "MobEnemy.h"

class CanonEnemy : public MobEnemy
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model_">使用するモデル</param>
	/// <param name="pos">初期位置</param>
	/// <returns></returns>
	static CanonEnemy* Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos);

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

private:
	// 出現演出中かどうか
	bool isAppear = false;

	// イージングタイマー
	float easeTimer = 0;

	// 上限
	const float easeTime = 30;

	// 最初のスケール
	KMyMath::Vector3 startPos;
};

