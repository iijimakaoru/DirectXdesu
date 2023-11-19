#include "Ground.h"
#include "Player.h"
#include "RailCamera.h"

void Ground::Init(Player* player_)
{
	// モデル生成
	model = std::make_unique<MtlObj>("Grand");
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
		object3d[i]->SetPos({ 0.0f,-20.0f,0.0f });
		object3d[i]->SetScale({ 1.0f,1.0f,1.0f });
	}

	object3d[1]->SetPos({ 0,-20,(object3d[0]->GetPos().z + 1800.0f) });

	player = player_;

	isAdvance = false;
}

void Ground::Update(ViewProjection* viewPro_, const KMyMath::Vector3& cameraPos_)
{
	const float flontOfScreenDiffuse = 1800;

	for (size_t i = 0; i < 2; i++)
	{
		if (!player->GetIsDead())
		{
			if (object3d[i]->GetPos().z + (flontOfScreenDiffuse * 2/3) <= cameraPos_.z)
			{
				if (i == 0)
				{
					object3d[0]->SetPos({ 0,-20,object3d[1]->GetPos().z + flontOfScreenDiffuse });
				}
				else if (i == 1)
				{
					object3d[1]->SetPos({ 0,-20,object3d[0]->GetPos().z + flontOfScreenDiffuse });
				}
			}
		}

		if (isAdvance)
		{
			object3d[i]->AddSetPos({ 0.0f,0.0f,-RailCamera::GetSpeed() * 2.0f });
		}
		else
		{
			object3d[i]->AddSetPos({ 0.0f,0.0f,-RailCamera::GetSpeed() });
		}

		object3d[i]->Update(viewPro_);
	}
}

void Ground::Draw()
{
	for (size_t i = 0; i < 2; i++)
	{
		object3d[i]->Draw();
	}
}

const KMyMath::Vector3 Ground::GetPos(size_t num_) const
{
	return object3d[num_]->GetPos();
}

void Ground::SetIsAdvance(const bool isAdvance_)
{
	isAdvance = isAdvance_;
}
