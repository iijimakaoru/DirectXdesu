#include "Player.h"

void Player::Init()
{
	input = KInput::GetInstance();

	// モデル生成
	model = std::make_unique<Cube>();
	model->CreateModel();

	// テクスチャ生成
	tex.CreateTexture("Resources/texture/", "mario.jpg");

	// パイプライン生成
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline = std::make_unique<KGPlin>();
	pipeline->CreatePipeline(shader, 5);

	// オブジェクト生成
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model.get());
}

void Player::Update(ViewProjection& viewPro)
{
	leftStickPos = input->GetPadLStick();
	if (leftStickPos.x != 0 && leftStickPos.y != 0)
	{
		Move();
	}

	object3d->Update(viewPro);
}

void Player::Move()
{
	
}

void Player::Draw()
{
	object3d->Draw(&tex);
}
