#include "Boss.h"
#include "BossActStand.h"
#include "BossActStamp.h"
#include "BossActAimShot.h"
#include "KInput.h"
#include "MyMath.h"

Boss* Boss::nowBoss = nullptr;

Boss::Boss(KModel* model)
{
	// �I�u�W�F�N�g�̏�����
	object.Initialize();
	// ���f���̓ǂݍ���
	model_ = model;
	object.LoadModel(model_);
	// �e�N�X�`��
	texture_.CreateTexture("Resources/texture/", "bossColor.png");
}

void Boss::Init()
{
	// �����ʒu
	object.transform.pos = { 0,20,100 };
	object.transform.rot = { 0,0,0 };
	object.transform.scale = { 20,20,20 };
	// HP
	hp = maxHP;

	startFlag = false;

	actState = nullptr;

	isAlive = true;
}

void Boss::Update(ViewProjection& viewProjection)
{
	if (startFlag && actState == nullptr)
	{
		actState = std::make_unique<BossActStand>();
	}

	actState->Update();

	object.Update(viewProjection);
}

void Boss::Draw()
{
	if (isAlive)
	{
		object.Draw(&texture_);
	}
}

void Boss::Damage()
{
	hp--;
}
