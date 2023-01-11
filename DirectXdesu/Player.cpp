#include "Player.h"
#include "KInput.h"
#include "Boss.h"
#include "Matrix4.h"
#include "ParticleManager.h"

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
	normal.CreateTexture("Resources/texture/", "playerColor.png");
	// カメラ
	view.Initialize();
	view.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();

	hp = maxHP;

	view.lenZ = -200;
}

void Player::Update(ViewProjection& viewProjection)
{
	moveVec = { 0,0,0 };

	Vector3 cameraVec =
	{
		view.target.x - view.eye.x,
		view.target.y - view.eye.y,
		view.target.z - view.eye.z
	};

	if (!isDash)
	{
		if (KInput::GetInstance()->GetPadConnect())
		{
			// カメラ操作
			rStick = KInput::GetInstance()->GetPadRStick();
			if (rStick.x != 0 || rStick.y != 0)
			{
				if (rStick.x > 0)
				{
					view.angleX -= XMConvertToRadians(rStick.x);
					object.transform.rot.y += XMConvertToRadians(rStick.x);
				}

				if (rStick.x < 0)
				{
					view.angleX += XMConvertToRadians(-rStick.x);
					object.transform.rot.y -= XMConvertToRadians(-rStick.x);
				}

				if (rStick.y > 0)
				{
					if (view.angleY < XMConvertToRadians(5.0f))
					{
						view.angleY += XMConvertToRadians(rStick.y);
					}
				}

				if (rStick.y < 0)
				{
					if (view.angleY > XMConvertToRadians(-45.0f))
					{
						view.angleY -= XMConvertToRadians(-rStick.y);
					}
				}
			}

			// プレイヤーの操作
			lStick = KInput::GetInstance()->GetPadLStick();
			if (lStick.x != 0 || lStick.y != 0)
			{
				float cameraRad = atan2f(cameraVec.x, cameraVec.z);
				float stickRad = atan2f(lStick.x, lStick.y);

				moveVec = { 0,0,1 };
				moveVec *= Matrix4::RotationY(cameraRad + stickRad);
				moveVec.Normalize();
				moveVec *= lStick.Length();

				object.transform.pos.x += moveVec.x * speed;
				object.transform.pos.z += moveVec.z * speed;
			}

			// angleラジアンy軸回転
			view.eye.x = view.lenZ * cosf(view.angleX) * cosf(view.angleY) + Player::nowPlayer->object.transform.pos.x;
			view.eye.y = (view.lenZ * sinf(view.angleY) + Player::nowPlayer->object.transform.pos.y) + 10;
			view.eye.z = view.lenZ * sinf(view.angleX) * cosf(view.angleY) + Player::nowPlayer->object.transform.pos.z;

			if (view.eye.y < -14)
			{
				view.eye.y = -10;
			}

			// 注視点
			view.target.x = Player::nowPlayer->object.transform.pos.x;
			view.target.y = Player::nowPlayer->object.transform.pos.y + 10;
			view.target.z = Player::nowPlayer->object.transform.pos.z;


			if (KInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A) && !isJump)
			{
				isJump = true;
				jumpPower = jumpPowerMax;
			}
		}
		else
		{
			// カメラ操作
			if (KInput::GetInstance()->IsPush(DIK_D) ||
				KInput::GetInstance()->IsPush(DIK_A) ||
				KInput::GetInstance()->IsPush(DIK_W) ||
				KInput::GetInstance()->IsPush(DIK_S))
			{
				if (KInput::GetInstance()->IsPush(DIK_D))
				{
					view.angleX += XMConvertToRadians(1.0f);
					object.transform.rot.y -= XMConvertToRadians(1.0f);
				}
				else if (KInput::GetInstance()->IsPush(DIK_A))
				{
					view.angleX -= XMConvertToRadians(1.0f);
					object.transform.rot.y += XMConvertToRadians(1.0f);
				}
				if (KInput::GetInstance()->IsPush(DIK_W))
				{
					if (view.angleY < XMConvertToRadians(5.0f))
					{
						view.angleY += XMConvertToRadians(1.0f);
					}
				}
				else if (KInput::GetInstance()->IsPush(DIK_S))
				{
					if (view.angleY > XMConvertToRadians(-45.0f))
					{
						view.angleY -= XMConvertToRadians(1.0f);
					}
				}
			}

			// angleラジアンy軸回転
			view.eye.x = view.lenZ * cosf(view.angleX) * cosf(view.angleY) + Player::nowPlayer->object.transform.pos.x;
			view.eye.y = (view.lenZ * sinf(view.angleY) + Player::nowPlayer->object.transform.pos.y) + 10;
			view.eye.z = view.lenZ * sinf(view.angleX) * cosf(view.angleY) + Player::nowPlayer->object.transform.pos.z;

			if (view.eye.y < -14)
			{
				view.eye.y = -10;
			}

			// 注視点
			view.target.x = Player::nowPlayer->object.transform.pos.x;
			view.target.y = Player::nowPlayer->object.transform.pos.y + 10;
			view.target.z = Player::nowPlayer->object.transform.pos.z;

			// プレイヤーの移動
			if (KInput::GetInstance()->IsPush(DIK_RIGHT) ||
				KInput::GetInstance()->IsPush(DIK_LEFT) ||
				KInput::GetInstance()->IsPush(DIK_UP) ||
				KInput::GetInstance()->IsPush(DIK_DOWN))
			{
				cameraVec.Normalize();

				Vector2 keyVec = {};
				if (KInput::GetInstance()->IsPush(DIK_RIGHT))
				{
					keyVec.x = 1.0f;
				}
				else if (KInput::GetInstance()->IsPush(DIK_LEFT))
				{
					keyVec.x = -1.0f;
				}
				else
				{
					keyVec.x = 0.0f;
				}

				if (KInput::GetInstance()->IsPush(DIK_UP))
				{
					keyVec.y = 1.0f;
				}
				else if (KInput::GetInstance()->IsPush(DIK_DOWN))
				{
					keyVec.y = -1.0f;
				}
				else
				{
					keyVec.y = 0.0f;
				}

				float cameraRad = atan2f(cameraVec.x, cameraVec.z);
				float keyRad = atan2f(keyVec.x, keyVec.y);

				moveVec = { 0,0,1 };
				moveVec *= Matrix4::RotationY(cameraRad + keyRad);
				moveVec.Normalize();

				object.transform.pos.x += moveVec.x * speed;
				object.transform.pos.z += moveVec.z * speed;
			}

			if (KInput::GetInstance()->IsTriger(DIK_SPACE) &&
				!isJump)
			{
				isJump = true;
				jumpPower = jumpPowerMax;
			}
		}
	}
	else
	{
		// angleラジアンy軸回転
		view.eye.x = view.lenZ * cosf(view.angleX) * cosf(view.angleY) + Player::nowPlayer->object.transform.pos.x;
		view.eye.y = (view.lenZ * sinf(view.angleY) + Player::nowPlayer->object.transform.pos.y) + 10;
		view.eye.z = view.lenZ * sinf(view.angleX) * cosf(view.angleY) + Player::nowPlayer->object.transform.pos.z;

		// 注視点
		view.target.x = Player::nowPlayer->object.transform.pos.x;
		view.target.y = Player::nowPlayer->object.transform.pos.y + 10;
		view.target.z = Player::nowPlayer->object.transform.pos.z;

		if (dashTimer > 0)
		{
			particleCoolTime--;
			if (particleCoolTime <= 0)
			{
				for (int i = 0; i < 2; i++)
				{
					ParticleManager::GetInstance()->Dash(object.transform.pos, { 5,5,5 }, { 1,1,1 }, 60, 5, moveVec);
				}
				particleCoolTime = 2;
			}
		}

		object.transform.pos += dashVec * (speed * 6);

		dashTimer--;

		if (dashTimer <= 0)
		{
			isDash = false;
		}
	}

	// 攻撃and移動
	if ((KInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER) ||
		KInput::GetInstance()->GetLTriggerDown()) && !isDash)
	{
		isDash = true;
		dashTimer = maxDashTimer;
		dashVec = moveVec;
	}

	// ジャンプ
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

	if (object.transform.pos.y > -4 && !isJump)
	{
		object.transform.pos.y -= 1;
	}

	float limit = 190;
	object.transform.pos.x = max(object.transform.pos.x, -limit);
	object.transform.pos.x = min(object.transform.pos.x, limit);
	object.transform.pos.z = max(object.transform.pos.z, -limit);
	object.transform.pos.z = min(object.transform.pos.z, limit);

	if (object.transform.pos.y <= -4)
	{
		object.transform.pos.y = -4;
	}

	view.Update();

	object.Update(view);
}

void Player::Draw()
{
	object.Draw(&normal);
}

void Player::Damage()
{
	Boss* boss = Boss::nowBoss;

	Vector3 smashVec = boss->object.transform.pos - object.transform.pos;

	smashVec.Normalize();

	object.transform.pos.x += -smashVec.x * 50;
	object.transform.pos.z += -smashVec.z * 50;
}

void Player::AttackHit()
{
	Boss* boss = Boss::nowBoss;

	Vector3 smashVec = boss->object.transform.pos - object.transform.pos;

	smashVec.Normalize();

	object.transform.pos.x += -smashVec.x * 12;
	object.transform.pos.y += smashVec.y * 12;
	object.transform.pos.z += -smashVec.z * 12;
}
