#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(KModel* model, KGPlin* pipeline_,
	const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed)
{
	// �C���X�^���X����
	PlayerBullet* playerBullet = new PlayerBullet();
	if (playerBullet == nullptr)
	{
		return nullptr;
	}

	// ������
	playerBullet->Init();
	playerBullet->Set(pos, vec_, rot_, bulletSpeed);

	return playerBullet;
}

void PlayerBullet::Init()
{
	// ���f������
	model = std::make_unique<Cube>();
	model->CreateModel();

	// �e�N�X�`������
	texData = TextureManager::Load("Resources/texture/mario.jpg");

	// �p�C�v���C������
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// �I�u�W�F�N�g����
	object3d.reset(KObject3d::Create(model.get(),pipeline.get()));
	object3d->transform.scale = { 3.0f,3.0f,3.0f };
}
