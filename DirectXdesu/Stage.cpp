#include "Stage.h"
#include "KTexture.h"

void Stage::Init(KModel* model)
{
	// �I�u�W�F�N�g�̏�����
	object.Initialize();
	object.transform.scale = { 200,1,200 };
	// ���f���̓ǂݍ���
	model_ = model;
	object.LoadModel(model_);
}

void Stage::Update(ViewProjection& viewProjection)
{
	object.transform.pos.y = -15;

	object.Update(viewProjection);
}

void Stage::Draw(KTexture* texture)
{
	object.Draw(texture);
}
