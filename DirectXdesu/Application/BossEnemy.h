#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "ViewProjection.h"
#include "TextureManager.h"

class BossEnemy
{
public:
	virtual void Init(KModel* model_) = 0;

	virtual void Update(ViewProjection* viewPro) = 0;

	virtual void Draw();

protected:
	std::unique_ptr<KObject3d> object3d = nullptr;

	KModel* model = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> pipeline;

	// シェーダー
	KShader shader;
};

