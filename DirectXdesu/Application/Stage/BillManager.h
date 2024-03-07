#pragma once
#include "Bill1.h"
#include "Camera.h"
#include "KGPlin.h"
#include "KModel.h"
#include "KObject3d.h"
#include "KShader.h"
#include <memory>
#include "Light.h"

/**
 * @file BillManager.h
 * @brief 建物マネージャー
 * @author 飯島 薫
 */

class BillManager {
public:
	BillManager();
	~BillManager();

	// 初期化
	void Init(Light* light);

	// 更新
	void Update(ViewProjection* viewPro,const KMyMath::Vector3& cameraPos, const float& cameraZ);

	// 描画
	void Draw();

	// ビルセット
	void Set(const float& cameraZ);
	void RightSet(const float cameraZ);
	void LeftSet(const float cameraZ);

	// 前進セッター
	void SetIsAdvance(const bool isAdvance_);

	// 生成止め
	void SetIsStopCreate(const bool isStopCreate_);

private:
	// ビル１
	std::list<std::unique_ptr<Bill1>> bill1s;

	// タイマー
	int32_t billtimer = 0;
	const int32_t billtime = 15;

	bool isAdvance = false;

	bool isStopCreate = false;

	Light* light_ = nullptr;
};
