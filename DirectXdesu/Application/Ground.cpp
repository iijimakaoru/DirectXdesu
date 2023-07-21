#include "Ground.h"

void Ground::Init()
{
	// モデル生成
	model = std::make_unique<Cube>();
	model->CreateModel();

	// テクスチャ生成
	texData = TextureManager::Load("Resources/texture/haikei.jpg");

	// パイプライン生成
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// オブジェクト生成
	for (size_t i = 0; i < 2; i++)
	{
		object3d[i] = std::make_unique<KObject3d>();
		object3d[i]->Initialize();
		object3d[i]->SetPipeline(pipeline.get());
		object3d[i]->LoadModel(model.get());
		object3d[i]->transform.pos = { 0,-20,0 };
		object3d[i]->transform.scale = { 150.0f,0.5f,300.0f };
	}

	object3d[1]->SetPos({ 0,-20,(object3d[0]->GetPos().z + object3d[0]->GetScale().z) * 2 });
}

void Ground::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos)
{
	const float flontOfScreenDiffuse = 100;
	const float swapPos = cameraPos.z + flontOfScreenDiffuse;

	for (size_t i = 0; i < 2; i++)
	{
		if (object3d[i]->GetPos().z + object3d[i]->GetScale().z * 2 <= cameraPos.z)
		{
			if (i == 0)
			{
				object3d[0]->SetPos({ 0,-20,object3d[1]->GetPos().z + object3d[1]->GetScale().z * 2 });
			}
			else if (i == 1)
			{
				object3d[1]->SetPos({ 0,-20,object3d[0]->GetPos().z + object3d[0]->GetScale().z * 2 });
			}
		}

		object3d[i]->Update(viewPro);
	}
}

void Ground::Draw()
{
	for (size_t i = 0; i < 2; i++)
	{
		object3d[i]->Draw(texData);
	}
}

const KMyMath::Vector3 Ground::GetPos(size_t num) const
{
	return object3d[num]->GetPos();
}
