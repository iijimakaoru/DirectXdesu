#include "MobEnemy.h"
#include "MyMath.h"

void MobEnemy::Init()
{
	// ���f������
	model = std::make_unique<MtlObj>("boss_model");
	model->CreateModel();

	// �e�N�X�`������
	tex.CreateTexture("Resources/texture/", "kitanai.jpg");

	// �p�C�v���C������
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// �I�u�W�F�N�g����
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model.get());
	object3d->transform.scale = { 4.0f,4.0f,4.0f };
}

void MobEnemy::Set()
{
	const float limitX = 60;
	const float limitY = 30;

	if (isDead)
	{
		object3d->transform.pos = { MyMathUtility::GetRand(-limitX,limitX),MyMathUtility::GetRand(-limitY,limitY),50 };
		speed = MyMathUtility::GetRand(0.3f, 0.5f);
		isDead = false;
	}
}

void MobEnemy::Update(ViewProjection* viewPro)
{
	Set();

	if (!isDead)
	{
		//object3d->transform.pos.z -= speed;

		if (object3d->transform.pos.z <= min(object3d->transform.pos.z, -100))
		{
			isDead = true;
		}
	}

	object3d->Update(viewPro);
}

void MobEnemy::Draw()
{
	if (!isDead)
	{
		object3d->Draw();
	}
}

void MobEnemy::OnCollision()
{
	isDead = true;
}

KMyMath::Vector3 MobEnemy::GetWorldPos()
{
	// ���[���h���W�i�[�ϐ�
	KMyMath::Vector3 result;

	// ���[���h�s��̕��s�ړ������擾
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}
