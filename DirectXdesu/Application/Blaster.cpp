#include "Blaster.h"

Blaster* Blaster::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos)
{
	// �C���X�^���X����
	Blaster* blaster = new Blaster();
	if (blaster == nullptr)
	{
		return nullptr;
	}

	// ������
	blaster->Init(model_, pipeline_, pos);

	return blaster;
}

void Blaster::Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos)
{
	input = KInput::GetInstance();

	// ���f������
	model = model_;

	// �p�C�v���C������
	pipeline = pipeline_;

	// �I�u�W�F�N�g����
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->transform.scale = { 10.0f,10.0f,10.0f };
	object3d->transform.pos = initPos;
}

void Blaster::Update(ViewProjection* viewPro)
{
	object3d->Update(viewPro);
}
