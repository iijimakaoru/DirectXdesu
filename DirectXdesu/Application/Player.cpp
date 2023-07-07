#include "Player.h"
#include "Ease.h"

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
	pipeline->CreatePipelineAll(shader, "Obj");

	// オブジェクト生成
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model.get());
	object3d->transform.pos.z = 50;
	object3d->transform.scale = { 2.0f,2.0f,2.0f };

	// 弾生成
	for (size_t i = 0; i < bullet.size(); i++)
	{
		bullet[i] = std::make_unique<Bullet>();
		bullet[i]->Init();
	}

	// レティクル
	reticle3d = std::make_unique<Reticle3D>();
	reticle3d->Init();
}

void Player::Update(ViewProjection* viewPro)
{
	Move();
	Rot();

	Attack();

	reticle3d->Update(viewPro, object3d->transform.matWorld, GetWorldPos());

	object3d->Update(viewPro);

	for (size_t i = 0; i < bullet.size(); i++)
	{
		bullet[i]->Update(viewPro);
	}
}

void Player::Move()
{
	if (input->IsPush(DIK_UP))
	{
		object3d->transform.pos.z++;
	}

	if (input->IsPush(DIK_DOWN))
	{
		object3d->transform.pos.z--;
	}

	//自機が傾いている角度に移動させる
	KMyMath::Vector3 velocity = { 0, 0, 0 };
	velocity.x = (object3d->transform.rot.y / rotLimit.y);
	velocity.y = -(object3d->transform.rot.x / rotLimit.x);

	object3d->transform.pos.x += velocity.x * moveSpeed;
	object3d->transform.pos.y += velocity.y * moveSpeed;

	const float moveLimitX = 60;
	const float moveLimitY = 30;

	object3d->transform.pos.x = max(object3d->transform.pos.x, -moveLimitX);
	object3d->transform.pos.x = min(object3d->transform.pos.x, moveLimitX);
	object3d->transform.pos.y = max(object3d->transform.pos.y, -moveLimitY);
	object3d->transform.pos.y = min(object3d->transform.pos.y, moveLimitY);
}

void Player::Rot()
{
	//回転速度
	const float rotSpeed = 0.5f;

	//角度修正基準速度
	const float correctionSpeed = rotSpeed / 6.0f;
	KMyMath::Vector3 rot = { 0, 0, 0 };

	//どこまで傾けたら判定をとるか
	const float stickNum = 200;

	// Y軸回転
	if (input->LStickTiltX(stickNum) || input->LStickTiltX(-stickNum))
	{
		//自機はスティックを倒した方向に動く
		const float stickRota = input->GetLStickAngle();
		const float moveAngle = XMConvertToRadians(stickRota);
		const float padStickIncline = input->GetLStickInline().x;
		rot.y = rotSpeed * cosf(moveAngle) * fabsf(padStickIncline);
	}
	else
	{
		//角度修正速度倍率
		float backSpeedRatio = fabsf(object3d->transform.rot.y / (rotLimit.y * 2)) + 0.5f;
		//角度修正速度
		const float backSpeed = correctionSpeed * backSpeedRatio;
		//y軸回転の傾きを修正する
		const float rotMin = 0.5f;
		if (object3d->transform.rot.y > rotMin)
		{
			rot.y -= backSpeed;
		}
		else if (object3d->transform.rot.y < -rotMin)
		{
			rot.y += backSpeed;
		}
		else
		{
			object3d->transform.rot.y = 0;
		}
	}

	// X軸回転
	if (input->LStickTiltY(stickNum) || input->LStickTiltY(-stickNum))
	{
		//自機はスティックを倒した方向に動く
		const float stickRota = -input->GetLStickAngle();
		const float moveAngle = XMConvertToRadians(stickRota);
		const float padStickIncline = input->GetLStickInline().y;
		rot.x = rotSpeed * sinf(moveAngle) * fabsf(padStickIncline);
	}
	else
	{
		//角度修正速度倍率
		float backSpeedRatio = fabsf(object3d->transform.rot.x / (rotLimit.x * 2)) + 0.5f;
		//角度修正速度
		const float backSpeed = correctionSpeed * backSpeedRatio;
		//y軸回転の傾きを修正する
		const float rotMin = 0.5f;
		if (object3d->transform.rot.x > rotMin)
		{
			rot.y -= backSpeed;
		}
		else if (object3d->transform.rot.x < -rotMin)
		{
			rot.y += backSpeed;
		}
		else
		{
			object3d->transform.rot.x = 0;
		}
	}

	object3d->transform.rot = rot;

	// 角度制限
	object3d->transform.rot.y = max(object3d->transform.rot.y, -rotLimit.y);
	object3d->transform.rot.y = min(object3d->transform.rot.y,  rotLimit.y);
	object3d->transform.rot.x = max(object3d->transform.rot.x, -rotLimit.x);
	object3d->transform.rot.x = min(object3d->transform.rot.x,  rotLimit.x);
}

void Player::Attack()
{
	if (input->GetPadButtonDown(XINPUT_GAMEPAD_A))
	{
		const float bulletSpeed = 6.0f;
		KMyMath::Vector3 bulletVec(0, 0, 1);

		// 速度ベクトルを自機の向きに合わせて回転
		bulletVec = MyMathUtility::TransforNormal(bulletVec, object3d->transform.matWorld);

		for (size_t i = 0; i < bullet.size(); i++)
		{
			if (bullet[i]->GetIsDead())
			{
				bullet[i]->Set(GetWorldPos(), bulletVec, object3d->transform.rot, bulletSpeed);
				return;
			}
		}
	}
}

void Player::Draw()
{
	reticle3d->Draw();

	object3d->Draw(&tex);
	
	for (size_t i = 0; i < bullet.size(); i++)
	{
		bullet[i]->Draw();
	}
}

void Player::SetParent(const WorldTransfom* parent)
{
	object3d->transform.parent = parent;
}
