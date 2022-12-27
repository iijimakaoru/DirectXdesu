#include "Player.h"
#include "KInput.h"

void Player::Init(KModel* model)
{
	// オブジェクトの初期化
	object.Initialize();
	object.transform.scale = { 10,10,10 };
	// モデルの読み込み
	model_ = model;
	object.LoadModel(model_);
	// 速度
	speed = 1;

	isJump = false;

	jumpPower = 0;
}

void Player::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	object.transform.pos.x += (KInput::GetInstance()->IsPush(DIK_RIGHT) - KInput::GetInstance()->IsPush(DIK_LEFT)) * speed;
	object.transform.pos.z += (KInput::GetInstance()->IsPush(DIK_UP) - KInput::GetInstance()->IsPush(DIK_DOWN)) * speed;

	if (KInput::GetInstance()->IsTriger(DIK_SPACE))
	{
		isJump = true;
		jumpPower = jumpPowerMax;
	}

	if (isJump)
	{
		object.transform.pos.y += jumpPower;
		jumpPower -= 0.2f;
		if (jumpPower < -jumpPowerMax)
		{
			isJump = false;
			jumpPower = 0;
		}
	}

	object.Update(matView, matProjection);
}

void Player::Draw()
{
	object.Draw();
}
