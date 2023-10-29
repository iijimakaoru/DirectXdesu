#pragma once
#include "BossEnemy.h"
#include "KInput.h"

/**
 * @file Blaster.h
 * @brief ボス1:ブラスター
 * @author 飯島 薫
 */

class Blaster : public BossEnemy
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="pipeline_"></param>
	/// <param name="pos"></param>
	/// <param name="HP"></param>
	/// <param name="spritePipeline_"></param>
	/// <returns></returns>
	static Blaster* Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos,
		const float HP, KGPlin* spritePipeline_);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="pipeline_"></param>
	/// <param name="initPos"></param>
	/// <param name="HP_"></param>
	/// <param name="spritePipeline_"></param>
	void Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos,
		const float HP_, KGPlin* spritePipeline_) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro) override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="posA"></param>
	/// <param name="posB"></param>
	/// <returns></returns>
	bool CollisionCheck(const KMyMath::Vector3& posA, const KMyMath::Vector3& posB) override;
};

