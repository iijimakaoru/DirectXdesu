#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"
#include "KTexture.h"

class Boss
{
public:
	static Boss* nowBoss;

	KObject3d object;

	KModel* model_;

	KTexture texture_;

	void Init(KModel* model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw();
};

