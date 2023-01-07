#include "Player.h"
#include "KInput.h"

Player* Player::nowPlayer = nullptr;

void Player::Init(KModel* model)
{
	// オブジェクトの初期化
	object.Initialize();
	object.transform.pos.y = -4;
	object.transform.scale = { 10,10,10 };
	// モデルの読み込み
	model_ = model;
	object.LoadModel(model_);
	// 速度
	speed = 1;
	// ジャンプ
	isJump = false;
	jumpPower = 0;
	// テクスチャ
	texture_.CreateTexture("Resources/texture/", "playerColor.png");
	// カメラ
	view.Initialize();
	view.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();

	view.lenZ = -150;
}

void Player::Update(ViewProjection& viewProjection)
{
	XMFLOAT3 moveVec = { 0,0,0 };

	if (KInput::GetInstance()->IsPush(DIK_D))
	{
		view.angleX += XMConvertToRadians(1.0f);
	}
	else if (KInput::GetInstance()->IsPush(DIK_A))
	{
		view.angleX -= XMConvertToRadians(1.0f);
	}
	if (KInput::GetInstance()->IsPush(DIK_W))
	{
		view.angleY += XMConvertToRadians(1.0f);
	}
	else if (KInput::GetInstance()->IsPush(DIK_S))
	{
		view.angleY -= XMConvertToRadians(1.0f);
	}
	// angleラジアンy軸回転
	view.eye.x = view.lenZ * cosf(view.angleX) * cosf(view.angleY) + Player::nowPlayer->object.transform.pos.x;
	view.eye.y = (view.lenZ * sinf(view.angleY) + Player::nowPlayer->object.transform.pos.y) + 5;
	view.eye.z = view.lenZ * sinf(view.angleX) * cosf(view.angleY) + Player::nowPlayer->object.transform.pos.z;

	view.target.x = Player::nowPlayer->object.transform.pos.x;
	view.target.y = Player::nowPlayer->object.transform.pos.y + 10;
	view.target.z = Player::nowPlayer->object.transform.pos.z;

	stick = KInput::GetInstance()->GetPadLStick();
	if (KInput::GetInstance()->GetPadConnect())
	{
		if (KInput::GetInstance()->GetPadLStick().x > 0.5)
		{
			object.transform.pos.x += speed;
		}
		else if (KInput::GetInstance()->GetPadLStick().x < -0.5)
		{
			object.transform.pos.x -= speed;
		}

		if (KInput::GetInstance()->GetPadLStick().y > 0.5)
		{
			object.transform.pos.z += speed;
		}
		else if (KInput::GetInstance()->GetPadLStick().y < -0.5)
		{
			object.transform.pos.z -= speed;
		}

		if (KInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A) && !isJump)
		{
			isJump = true;
			jumpPower = jumpPowerMax;
		}
	}
	else
	{
		object.transform.pos.x += (KInput::GetInstance()->IsPush(DIK_RIGHT) - KInput::GetInstance()->IsPush(DIK_LEFT)) * speed;
		object.transform.pos.z += (KInput::GetInstance()->IsPush(DIK_UP) - KInput::GetInstance()->IsPush(DIK_DOWN)) * speed;

		if (KInput::GetInstance()->IsTriger(DIK_SPACE) && !isJump)
		{
			isJump = true;
			jumpPower = jumpPowerMax;
		}
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

	float limit = 190;
	object.transform.pos.x = max(object.transform.pos.x, -limit);
	object.transform.pos.x = min(object.transform.pos.x, limit);
	object.transform.pos.z = max(object.transform.pos.z, -limit);
	object.transform.pos.z = min(object.transform.pos.z, limit);

	object.Update(view);

	view.Update();
}

void Player::Draw()
{
	object.Draw(&texture_);
}
