#include "Player.h"
#include "Ease.h"

const float Player::moveSpeed = 0.32f;
const KMyMath::Vector2 Player::rotLimit = { 35.0f, 25.0f };
const KMyMath::Vector2 Player::posLimitMin = { -15.0f, -4.0f };
const KMyMath::Vector2 Player::posLimitMax = { 15.0f, Player::posLimitMin.y + 12.0f };

void Player::Init(KModel* model_)
{
	input = KInput::GetInstance();

	// モデル生成
	model = model_;

	// パイプライン生成
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// オブジェクト生成
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model);
	object3d->transform.pos.z = 50;
	object3d->transform.scale = { 2.0f,2.0f,2.0f };

	// レティクル
	reticle3d = std::make_unique<Reticle3D>();
	reticle3d->Init();
	reticle2d = std::make_unique<Reticle2D>();
	reticle2d->Init();
}

void Player::Update(ViewProjection* viewPro)
{
	// 弾削除
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->GetIsDead();
		});

	Move();
	Rot();

	Attack();

	reticle3d->Update(object3d->transform.matWorld, GetWorldPos());

	reticle2d->Update(viewPro,reticle3d->GetWorldPos());

	object3d->Update(viewPro);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Update(viewPro);
	}
}

void Player::Move()
{
	//自機が傾いている角度に移動させる
	KMyMath::Vector3 velocity = { 0, 0, 0 };
	velocity.x = (object3d->transform.rot.y / rotLimit.y);
	velocity.y = -(object3d->transform.rot.x / rotLimit.x);

	object3d->transform.pos.x += velocity.x * moveSpeed;
	object3d->transform.pos.y += velocity.y * moveSpeed;

	object3d->transform.pos.x = max(object3d->transform.pos.x, posLimitMin.x);
	object3d->transform.pos.x = min(object3d->transform.pos.x, posLimitMax.x);
	object3d->transform.pos.y = max(object3d->transform.pos.y, posLimitMin.y);
	object3d->transform.pos.y = min(object3d->transform.pos.y, posLimitMax.y);
}

void Player::Rot()
{
	//回転速度
	const float rotSpeed = 0.025f;

	//角度修正基準速度
	const float correctionSpeed = (rotSpeed * 2.0f) / 0.1f;
	KMyMath::Vector3 rot = { 0, 0, 0 };

	//どこまで傾けたら判定をとるか
	const float stickNum = 100;

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
			rot.x -= backSpeed;
		}
		else if (object3d->transform.rot.x < -rotMin)
		{
			rot.x += backSpeed;
		}
		else
		{
			object3d->transform.rot.x = 0;
		}
	}

	// Z軸回転
	{
		const float rotZspeed = 0.01f;
		const float rotZLimit = 1.0f;
		//右回転
		if (isRotZRight) 
		{
			swayZ += rotZspeed;
			if (swayZ >= rotZLimit) 
			{
				isRotZRight = false;
			}
		}
		//左回転
		else 
		{
			swayZ -= rotZspeed;
			if (swayZ <= -rotZLimit) 
			{
				isRotZRight = true;
			}
		}

		object3d->transform.rot.z = -object3d->transform.rot.y + swayZ;
	}

	object3d->transform.rot += rot;

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

		// 弾生成
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		// 初期化
		newBullet->Init();
		// 配置
		newBullet->Set(GetWorldPos(), bulletVec, object3d->transform.rot, bulletSpeed);
		// 登録
		bullets.push_back(std::move(newBullet));
	}
}

void Player::Draw()
{
	object3d->Draw();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Draw();
	}

	reticle2d->Draw();
}

void Player::SetParent(const WorldTransfom* parent)
{
	object3d->transform.parent = parent;
}

KMyMath::Vector3 Player::GetWorldPos()
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}

void Player::OnCollision()
{
}
