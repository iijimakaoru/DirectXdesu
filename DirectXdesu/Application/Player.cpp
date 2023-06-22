#include "Player.h"

void Player::Init()
{
	input = KInput::GetInstance();

	// ���f������
	model = std::make_unique<Cube>();
	model->CreateModel();

	// �e�N�X�`������
	tex.CreateTexture("Resources/texture/", "mario.jpg");

	// �p�C�v���C������
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline = std::make_unique<KGPlin>();
	pipeline->CreatePipeline(shader, 5);

	// �I�u�W�F�N�g����
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
