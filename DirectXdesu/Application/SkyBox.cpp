#include "SkyBox.h"

SkyBox* SkyBox::Create(KModel* model_, KGPlin* objPipeline_, const float posZ)
{
	// �C���X�^���X����
	SkyBox* skyBox = new SkyBox();
	if (skyBox == nullptr)
	{
		return nullptr;
	}

	// ������
	skyBox->Init(model_, objPipeline_, posZ);

	return skyBox;
}

void SkyBox::Init(KModel* model_, KGPlin* objPipeline_, const float posZ)
{
	// ���f������
	model = model_;

	// �p�C�v���C������
	objPipeline = objPipeline_;

	// �I�u�W�F�N�g����
	object3d.reset(KObject3d::Create(model, objPipeline));
	object3d->transform.pos.z = posZ;
	object3d->transform.scale = { 600.0f,600.0f,600.0f };
}

void SkyBox::Update(ViewProjection* viewPro)
{
	object3d->Update(viewPro);
}

void SkyBox::ObjDraw()
{
	object3d->Draw();
}

void SkyBox::SetPosZ(const float posZ)
{
	object3d->transform.pos.z = posZ;
}
