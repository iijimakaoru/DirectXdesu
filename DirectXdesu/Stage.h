#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"
#include "ViewProjection.h"

class Stage
{
public:
	void Init(KModel* model);
	void Update(ViewProjection& viewProjection);
	void Draw(KTexture* texture);

	KObject3d object;

	KModel* model_;
};

