#include "Stage.h"
#include "KTexture.h"

void Stage::Init(KModel* model)
{
	// �I�u�W�F�N�g�̏�����
	object.Initialize();
	object.transform.scale = { 40,1,40 };
	// ���f���̓ǂݍ���
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
