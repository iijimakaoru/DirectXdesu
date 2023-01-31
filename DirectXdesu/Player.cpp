#include "Player.h"
#include "KInput.h"
#include "Boss.h"
#include "Matrix4.h"
#include "ParticleManager.h"
#include "KInput.h"

Player* Player::nowPlayer = nullptr;

Player::Player()
{
	
}

void Player::Init(KModel* model, KGPlin* pipeline)
{
	object = std::make_unique<KObject3d>();
	object->Initialize();
	object->SetPipeline(pipeline);
	object->LoadModel(model);
	object->transform.scale = { 10,10,10 };
}

void Player::Update(ViewProjection& viewProjection)
{
	KInput* input = KInput::GetInstance();

	if (input->IsPush(DIK_RIGHT))
	{
		object->transform.pos.x += 1.0f;
	}
	if (input->IsPush(DIK_LEFT))
	{
		object->transform.pos.x -= 1.0f;
	}

	if (input->IsPush(DIK_UP))
	{
		object->transform.pos.z += 1.0f;
	}
	if (input->IsPush(DIK_DOWN))
	{
		object->transform.pos.z -= 1.0f;
	}
	
	object->Update(viewProjection);
}

void Player::Draw()
{
	object->Draw();
}

void Player::Damage()
{
	
}

void Player::AttackHit()
{
	
}
