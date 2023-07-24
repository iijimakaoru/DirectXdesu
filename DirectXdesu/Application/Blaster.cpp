#include "Blaster.h"

void Blaster::Init(KModel* model_, const KMyMath::Vector3& initPos)
{
	input = KInput::GetInstance();

	// モデル生成
	model = model_;

	// パイプライン生成
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// オブジェクト生成
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model);
	object3d->transform.scale = { 10.0f,10.0f,10.0f };
	object3d->transform.pos = initPos;
}

void Blaster::Update(ViewProjection* viewPro)
{
	object3d->Update(viewPro);
}
