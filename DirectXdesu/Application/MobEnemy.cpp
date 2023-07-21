#include "MobEnemy.h"
#include "MyMath.h"

void MobEnemy::Init(KModel* model_)
{
	// ���f������
	model = model_;

	// �p�C�v���C������
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// �I�u�W�F�N�g����
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model);
	object3d->transform.scale = { 4.0f,4.0f,4.0f };
}

void MobEnemy::Set(KMyMath::Vector3& pos)
{
	object3d->transform.pos = pos;
	speed = MyMathUtility::GetRand(0.3f, 0.5f);
	isDead = false;
}

void MobEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	if (!isDead)
	{
		if (object3d->transform.pos.z <= min(object3d->transform.pos.z ,cameraPos.z))
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