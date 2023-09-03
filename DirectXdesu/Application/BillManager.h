#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"
#include "Bill1.h"

class BillManager
{
public:
	// 初期化
	void Init();

	// 更新
	void Update(ViewProjection* viewPro, const float& cameraZ);

	// 描画
	void Draw();

	// ビルセット
	void Set(const float& cameraZ);
	void RightSet(const float cameraZ);
	void LeftSet(const float cameraZ);

private:
	// モデル
	std::unique_ptr<KModel> model = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> objPipeline;

	// シェーダー
	KShader objShader;

	// ビル１
	std::list<std::unique_ptr<Bill1>> bill1s;

	// タイマー
	int32_t billtimer = 0;
	const int32_t billtime = 120;
};

