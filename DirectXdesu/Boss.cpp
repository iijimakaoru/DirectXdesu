#include "Boss.h"
#include "BossActStand.h"

Boss* Boss::nowBoss = nullptr;

void Boss::Init(KModel* model)
{
	// �I�u�W�F�N�g�̏�����
	object.Initialize();
	object.transform.pos.y = 20;
	object.transform.pos.z = 100;
	object.transform.scale = { 20,20,20 };
	// ���f���̓ǂݍ���
	model_ = model;
	object.LoadModel(model_);
	// �e�N�X�`��
	texture_.CreateTexture("Resources/texture/", "bossColor.png");
}

void Boss::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	if (startFlag && actState == nullptr)
	{
		actState = std::make_unique<BossActStand>();
	}

	actState->Update();

	object.Update(matView, matProjection);
}

void Boss::Draw()
{
	object.Draw(&texture_);
}
