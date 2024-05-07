#pragma once
#include "BossEnemy.h"
#include "KInput.h"
#include "Reticle2D.h"
#include "Reticle3D.h"

/**
 * @file Blaster.h
 * @brief ボス1:ブラスター
 * @author 飯島 薫
 */

class Blaster : public BossEnemy {
public:
	static Blaster* nowBlaster;

	static std::unique_ptr<ActState> blasterActState;

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
	static Blaster* Create(KGPlin* pipeline_, const KMyMath::Vector3& pos, KGPlin* spritePipeline_);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="pipeline_"></param>
	/// <param name="initPos"></param>
	/// <param name="HP_"></param>
	/// <param name="spritePipeline_"></param>
	void Init(KGPlin* pipeline_, const KMyMath::Vector3& initPos, KGPlin* spritePipeline_) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(
	    ViewProjection* viewPro, const KMyMath::Vector3& cameraPos, bool isBossMovie_) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	void UIDraw() override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="posA"></param>
	/// <param name="posB"></param>
	/// <returns></returns>
	bool CollisionCheck(const KMyMath::Vector3& posA, const KMyMath::Vector3& posB) override;

	void SetFarstAct() override;

	/// <summary>
	/// ユニットのポジションゲッター
	/// </summary>
	/// <returns></returns>
	const KMyMath::Vector3 GetUnitsPos(size_t num_) const;

	/// <summary>
	/// ユニットのスケールゲッター
	/// </summary>
	/// <param name="num_"></param>
	/// <returns></returns>
	const KMyMath::Vector3 GetUnitsScale(size_t num_) const;

	/// <summary>
	/// ユニットのポスセッター(0～7)
	/// </summary>
	/// <param name="pos_"></param>
	/// <param name="num_"></param>
	void SetUnitsPos(const KMyMath::Vector3& pos_, size_t num_);

	/// <summary>
	/// ユニットのスケールセッター
	/// </summary>
	/// <param name="scale_"></param>
	/// <param name="num_"></param>
	void SetUnitsScale(const KMyMath::Vector3& scale_, size_t num_);

	void AddSetPos(const KMyMath::Vector3& pos_);

	// ユニットワールドポス
	const KMyMath::Vector3 UnitsGetWorldPos(uint32_t& i) const;

private:
	std::array<std::unique_ptr<KObject3d>, 8> units;
	std::array<KMyMath::Vector3, 8> unitsPos;

	bool isStand = false;

	size_t actSelect = 0;
public:
	// 最終的なレティクル
	std::unique_ptr<Reticle2D> reticle2d = nullptr;
	bool isReticle = true;

	//
	ViewProjection* viewPro = nullptr;
};
