#include "PlayerBullet.h"

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
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model.get());
	object3d->transform.scale = { 3.0f,3.0f,3.0f };
}
