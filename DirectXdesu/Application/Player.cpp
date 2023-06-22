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
	pipeline->CreatePipelineAll(shader, true);

	// オブジェクト生成
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model.get());
	object3d->transform.scale = { 2.0f,2.0f,2.0f };
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
	object3d->transform.pos.x += (leftStickPos.x * speed) * leftStickPos.Length();
	object3d->transform.pos.y += (leftStickPos.y * speed) * leftStickPos.Length();

	const float moveLimitX = 60;
	const float moveLimitY = 30;

	object3d->transform.pos.x = max(object3d->transform.pos.x, -moveLimitX);
	object3d->transform.pos.x = min(object3d->transform.pos.x, moveLimitX);
	object3d->transform.pos.y = max(object3d->transform.pos.y, -moveLimitY);
	object3d->transform.pos.y = min(object3d->transform.pos.y, moveLimitY);
}

void Player::Draw()
{
	object3d->Draw(&tex);
}
