#include "Player.h"
#include "KInput.h"

void Player::Init(KModel* model)
{
	// �I�u�W�F�N�g�̏�����
	object.Initialize();
	object.transform.scale = { 10,10,10 };
	// ���f���̓ǂݍ���
	model_ = model;
	object.LoadModel(model_);
	// ���x
	speed = 1;
}

void Player::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	object.transform.pos.x += (KInput::GetInstance()->IsPush(DIK_RIGHT) - KInput::GetInstance()->IsPush(DIK_LEFT)) * speed;
	object.transform.pos.z += (KInput::GetInstance()->IsPush(DIK_UP) - KInput::GetInstance()->IsPush(DIK_DOWN)) * speed;

	object.Update(matView, matProjection);
}

void Player::Draw()
{
	object.Draw();
}
