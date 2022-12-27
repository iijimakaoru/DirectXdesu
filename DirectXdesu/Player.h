#pragma once
#include "KObject3d.h"
#include "KModel.h"

class Player
{
public:
	void Init(KModel* model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw();

private:
	KObject3d object;

	KModel* model_;

	float speed;
};

