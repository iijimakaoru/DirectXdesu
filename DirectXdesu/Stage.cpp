#include "Stage.h"
#include "KTexture.h"

void Stage::Init(KModel* model)
{
	// オブジェクトの初期化
	object.Initialize();
	object.transform.scale = { 200,1,200 };
	// モデルの読み込み
	model_ = model;
	object.LoadModel(model_);
}

void Stage::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	object.transform.pos.y = -15;

	object.Update(matView, matProjection);
}

void Stage::Draw(KTexture* texture)
{
	object.Draw(texture);
}
