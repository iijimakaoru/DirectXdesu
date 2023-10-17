#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"

/**
 * @file SkyBox.h
 * @brief 天箱
 * @author 飯島 薫
 */

class SkyBox
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="objPipeline_"></param>
	/// <param name="posZ"></param>
	/// <returns></returns>
	static SkyBox* Create(KModel* model_, KGPlin* objPipeline_, const float posZ);

public:
	// 初期化
	void Init(KModel* model_, KGPlin* objPipeline_, const float posZ);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera_"></param>
	void Update(ViewProjection* viewPro);

	// 描画
	// オブジェクト
	void ObjDraw();

	/// <summary>
	/// z座標セッター
	/// </summary>
	/// <param name="posZ"></param>
	void SetPosZ(const float posZ);

private:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	KModel* model = nullptr;

	// パイプライン
	KGPlin* objPipeline;
};

