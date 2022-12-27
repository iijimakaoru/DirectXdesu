#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"

class Stage
{
public:
	void Init(KModel* model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw(KTexture* texture);

	KObject3d object;

	KModel* model_;
};

