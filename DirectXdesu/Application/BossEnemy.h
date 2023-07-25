#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "ViewProjection.h"
#include "TextureManager.h"

class BossEnemy
{
public:
	virtual void Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos) = 0;

	virtual void Update(ViewProjection* viewPro) = 0;

	virtual void Draw();

protected:
	std::unique_ptr<KObject3d> object3d = nullptr;

	KModel* model = nullptr;

	// パイプライン
	KGPlin* pipeline;
};

