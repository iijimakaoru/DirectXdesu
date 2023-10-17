#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"

/**
 * @file Bill1.h
 * @brief 建物(ビル)
 * @author 飯島 薫
 */

class Bill1
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="objPipeline_"></param>
	/// <returns></returns>
	static Bill1* Create(KModel* model_, KGPlin* objPipeline_, const KMyMath::Vector2& pos);

public:
	Bill1();
	~Bill1();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="objPipeline_"></param>
	void Init(KModel* model_, KGPlin* objPipeline_, const KMyMath::Vector2 pos);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro, const float& cameraZ, const bool& isAdvance_);

	// 描画
	void Draw();

	// ゲッター
	const bool GetIsDead() const;

private:
	// オブジェクト
	std::unique_ptr<KObject3d> object;

	// モデル
	KModel* model = nullptr;

	// パイプライン
	KGPlin* objPipeline;

	bool isDead = false;
};

