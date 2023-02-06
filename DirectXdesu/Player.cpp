#include "Player.h"
#include "KInput.h"
#include "Boss.h"
#include "Matrix4.h"
#include "ParticleManager.h"
#include "Ease.h"

Player* Player::nowPlayer = nullptr;

Player::Player(KModel* model)
{
	// オブジェクトの初期化
	object.Initialize();
	// モデルの読み込み
	model_ = model;
	object.LoadModel(model_);
	// テクスチャ
	normal.CreateTexture("Resources/texture/", "playerColor.png");
	// カメラ
	view.Initialize();
	view.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();

	view.lenZ = -200;

	// 弾
	bulletObj1 = std::make_unique<KObject3d>();
	bulletObj1->Initialize();
	bulletObj1->LoadModel(model_);

	bulletObj2 = std::make_unique<KObject3d>();
	bulletObj2->Initialize();
	bulletObj2->LoadModel(model_);

	bulletObj3 = std::make_unique<KObject3d>();
	bulletObj3->Initialize();
	bulletObj3->LoadModel(model_);
}

void Player::Init()
{
	object.transform.pos = { 0,-4,0 };
	object.transform.rot = { 0,0,0 };
	object.transform.scale = { 10,10,10 };

	bulletObj1->transform.scale = { 5,5,5 };
	bulletObj2->transform.scale = { 5,5,5 };
	bulletObj3->transform.scale = { 5,5,5 };

	view.Initialize();

	// 速度
	speed = 1;
	// ジャンプ
	isJump = false;
	jumpPower = 0;

	particleCoolTime = 0;

	isHit = false;

	hitTimer = 0;

	isDamage = false;

	damageTimer = 0;

	isDash = false;

	isAlive = true;

	hp = maxHP;

	start = object.transform.pos;
	p1 = { object.transform.pos.x + 10,object.transform.pos.y - 10, object.transform.pos.z - 10 };
	p2 = { object.transform.pos.x,object.transform.pos.y - 10, object.transform.pos.z - 10 };
	p3 = { object.transform.pos.x + 10,object.transform.pos.y - 10, object.transform.pos.z - 10 };
	end = { object.transform.pos.x, object.transform.pos.y, object.transform.pos.z + 20 };

	nowTime = 0;
}

void Player::Update(ViewProjection& viewProjection)
{
	start = object.transform.pos;
	p1 = { object.transform.pos.x + 50,object.transform.pos.y + 30, object.transform.pos.z - 100 };
	p2 = { object.transform.pos.x,object.transform.pos.y + 50, object.transform.pos.z - 100 };
	p3 = { object.transform.pos.x - 50,object.transform.pos.y + 30, object.transform.pos.z - 100 };
	end = { object.transform.pos.x, object.transform.pos.y + 10, object.transform.pos.z + 120 };

	if (KInput::GetInstance()->GetPadConnect())
	{
		if (KInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B) && !isDrawBullet)
		{
			nowTime = 0;
			isDrawBullet = true;
		}
	}
	if (isDrawBullet)
	{
		if (nowTime < maxTime)
		{
			nowTime++;
		}
		else if (nowTime >= maxTime)
		{
			isDrawBullet = false;
		}

		Vector3 a = lerp3D(start, p1, nowTime / maxTime);
		Vector3 b = lerp3D(p1, end, nowTime / maxTime);

		Vector3 c = lerp3D(start, p2, nowTime / maxTime);
		Vector3 d = lerp3D(p2, end, nowTime / maxTime);

		Vector3 e = lerp3D(start, p3, nowTime / maxTime);
		Vector3 f = lerp3D(p3, end, nowTime / maxTime);

		bulletObj1->transform.pos = lerp3D(a, b, nowTime / maxTime);
		bulletObj2->transform.pos = lerp3D(c, d, nowTime / maxTime);
		bulletObj3->transform.pos = lerp3D(e, f, nowTime / maxTime);
		
		bulletObj1->Update(view);
		bulletObj2->Update(view);
		bulletObj3->Update(view);
	}

	moveVec = { 0,0,0 };

	Vector3 cameraVec =
	{
		view.target.x - view.eye.x,
		view.target.y - view.eye.y,
		view.target.z - view.eye.z
	};

	// カメラ操作
	if (KInput::GetInstance()->GetPadConnect())
	{
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

	if (!isDash)
	{
		if (KInput::GetInstance()->GetPadConnect())
		{
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

			if (KInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A) && !isJump)
			{
				isJump = true;
				jumpPower = jumpPowerMax;
			}
		}
	}
	else
	{
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
	if (KInput::GetInstance()->GetPadConnect())
	{
		if ((KInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER) ||
			KInput::GetInstance()->GetLTriggerDown()) && !isDash)
		{
			isDash = true;
			dashTimer = maxDashTimer;
			dashVec = moveVec;
		}
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

	if (isDamage)
	{
		Damage();
	}

	if (isHit)
	{
		AttackHit();
	}

	view.Update();

	object.Update(view);
}

void Player::Draw()
{
	if (isDrawBullet)
	{
		bulletObj1->Draw(&normal);
		bulletObj2->Draw(&normal);
		bulletObj3->Draw(&normal);
	}

	if (isAlive)
	{
		object.Draw(&normal);
	}
}

void Player::Damage()
{
	Boss* boss = Boss::nowBoss;

	Vector3 smashVec = boss->object.transform.pos - object.transform.pos;

	smashVec.Normalize();

	if (--damageTimer > 0)
	{
		object.transform.pos.x += -smashVec.x * 10;
		object.transform.pos.z += -smashVec.z * 10;
	}
	else
	{
		hp--;
		isDamage = false;
	}
}

void Player::AttackHit()
{
	Boss* boss = Boss::nowBoss;

	Vector3 smashVec = boss->object.transform.pos - object.transform.pos;

	smashVec.Normalize();

	if (--hitTimer > 0)
	{
		object.transform.pos.x += -smashVec.x * 10;
		object.transform.pos.y += smashVec.y * 10;
		object.transform.pos.z += -smashVec.z * 10;
	}
	else
	{
		isHit = false;
	}
}
