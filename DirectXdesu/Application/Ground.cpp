#include "Ground.h"

void Ground::Init()
{
	// ���f������
	model = std::make_unique<Cube>();
	model->CreateModel();

	// �e�N�X�`������
	texData = TextureManager::Load("Resources/texture/haikei.jpg");

	// �p�C�v���C������
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// �I�u�W�F�N�g����
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model.get());
	object3d->transform.pos = { 0,-30,0 };
	object3d->transform.scale = { 150.0f,0.5f,300.0f };
}

void Ground::Update(ViewProjection* viewPro)
{
	object3d->Update(viewPro);
}

void Ground::Draw()
{
	object3d->Draw(texData);
}
