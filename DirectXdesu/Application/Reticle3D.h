#pragma once
#include "KObject3d.h"
#include "KGPlin.h"
#include "KShader.h"

class Reticle3D
{
public:
	void Init();

	void Update(ViewProjection* viewProjection, KMyMath::Matrix4& m, KMyMath::Vector3 pPos);

	void Draw();

private:
	std::unique_ptr<KObject3d> object = nullptr;

	std::unique_ptr<KGPlin> pipeline = nullptr;

	KShader shader;

	KTexture tex;

	std::unique_ptr<KModel> model = nullptr;
};