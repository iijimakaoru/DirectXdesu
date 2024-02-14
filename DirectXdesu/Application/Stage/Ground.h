#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "KModel.h"
#include "KObject3d.h"
#include "KShader.h"
#include <memory>

/**
 * @file Ground.h
 * @brief 地面
 * @author 飯島 薫
 */

class Player;

class Ground {
public:
	// 初期化
	void Init(Player* player_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos);

	// 描画
	void Draw();

	// ゲッター
	const KMyMath::Vector3 GetPos(size_t num) const;

	// 前進セッター
	void SetIsAdvance(const bool isAdvance_);

private:
	// オブジェクト
	std::array<std::unique_ptr<KObject3d>, 2> object3d;

	// プレイヤーインスタンス格納
	Player* player;

	// そのまま動き続けろフラグ
	bool isAdvance = false;
};