#pragma once
#include "MobEnemy.h"

/**
 * @file FlyEnemy.h
 * @brief 前方に張り付く敵
 * @author 飯島 薫
 */

class FlyEnemy : public MobEnemy {
public:
	enum Mode { AppearMode, BattleMode, WithdrawalMode };

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="pipeline_"></param>
	/// <param name="startPos_"></param>
	/// <param name="endPos_"></param>
	/// <param name="speed_"></param>
	/// <returns></returns>
	static FlyEnemy* Create(
	    KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos_,
	    const KMyMath::Vector2& endPos_, const float& speed_, const uint32_t& lifeTime_,
	    const KMyMath::Vector2& wStartPos_, const KMyMath::Vector2& wEndPos_);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	void Init(KModel* model_, KGPlin* pipeline_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	/// <param name="cameraPos"></param>
	void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack() override;

	/// <summary>
	/// 出現
	/// </summary>
	void Appear();

	/// <summary>
	/// 戦闘
	/// </summary>
	void Battle(const KMyMath::Vector3& cameraPos);

	/// <summary>
	/// 撤退
	/// </summary>
	void Withdrawal();

private:
	/// <summary>
	/// スタートポジション
	/// </summary>
	/// <param name="startPos_"></param>
	void SetStartPos(const KMyMath::Vector3& startPos_);
	void SetWStartPos(const KMyMath::Vector2& wStartPos_);

	/// <summary>
	/// エンドポジション
	/// </summary>
	/// <param name="endPos_"></param>
	void SetEndPos(const KMyMath::Vector2& endPos_);
	void SetWEndPos(const KMyMath::Vector2& wEndPos_);

	/// <summary>
	/// スピードセット
	/// </summary>
	/// <param name="speed"></param>
	void SetSpeed(const float& speed);

	void SetLifeTime(const uint32_t& lifeTime_);

private:
	Mode enemyMode = Mode::AppearMode;

	// イージングタイマー
	float easeTimer = 0;

	// 上限
	const float easeTime = 30;

	// 最初の場所
	KMyMath::Vector3 startPos;
	KMyMath::Vector2 wStartPos;

	// 最終の場所
	KMyMath::Vector2 endPos;
	KMyMath::Vector2 wEndPos;

	// 出現演出中かどうか
	bool isAppear = false;

	bool isWithdrawal = false;

	// スピード
	float moveSpeed = 0;

	// 生存時間
	uint32_t lifeTime = 0;
	uint32_t lifeTimer = 0;
};
