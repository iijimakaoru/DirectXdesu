#include "Blaster.h"

void Blaster::Init(KModel* model_)
{
	input = KInput::GetInstance();

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
	object3d->transform.scale = { 10.0f,10.0f,10.0f };
}

void Blaster::Update(ViewProjection* viewPro)
{

	object3d->Update(viewPro);
}
