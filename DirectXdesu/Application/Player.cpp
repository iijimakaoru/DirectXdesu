#include "Player.h"
#include "Ease.h"
#include "BulletManager.h"
#include "ParticleManager.h"

const float Player::moveSpeed = 0.48f;
const KMyMath::Vector2 Player::rotLimit = { 35.0f, 25.0f };
const KMyMath::Vector2 Player::posLimitMin = { -15.0f, -4.0f };
const KMyMath::Vector2 Player::posLimitMax = { 15.0f, Player::posLimitMin.y + 12.0f };

Player* Player::Create(KModel* model_, KGPlin* objPipeline_, const float playerHP, KGPlin* spritePipeline_)
{
	// インスタンス生成
	Player* player = new Player();
	if (player == nullptr)
	{
		return nullptr;
	}

	// 初期化
	player->Init(model_, objPipeline_, playerHP, spritePipeline_);

	return player;
}

void Player::Init(KModel* model_, KGPlin* objPipeline_, const float playerHP, KGPlin* spritePipeline_)
{
	input = KInput::GetInstance();

	// モデル生成
	model = model_;

	// パイプライン生成
	objPipeline = objPipeline_;
	spritePipeline = spritePipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, objPipeline));
	object3d->transform.pos.z = 50;
	object3d->transform.scale = { 2.0f,2.0f,2.0f };

	// レティクル
	reticle3d = std::make_unique<Reticle3D>();
	reticle3d->Init();
	reticle2d = std::make_unique<Reticle2D>();
	reticle2d->Init();

	// HP関連
	maxHP = playerHP;
	HP = maxHP;

	HPUI = std::make_unique<Sprite>();
	HPUI->Init();
	HPUI->SetPipeline(spritePipeline);

	hpTex = TextureManager::Load("Resources/texture/white1x1.png");

	HPBarUI = std::make_unique<Sprite>();
	HPBarUI->Init();
	HPBarUI->SetPipeline(spritePipeline);

	hpbarTex = TextureManager::Load("Resources/texture/PlayersHPBar.png");

	isDead = false;
}

void Player::Update(ViewProjection* viewPro)
{
	// 死亡条件
	if (HP <= min(HP, 0))
	{
		isDead = true;
	}

	if (!isDead)
	{
		// 移動
		Move();

		// 回転
		Rot();

		// 攻撃
		Attack();
	}
	else
	{
		DeadEffect();
	}

	// 3Dレティクルの更新
	reticle3d->Update(object3d->transform.matWorld, GetWorldPos());

	// 2Dレティクルの更新
	reticle2d->Update(viewPro, reticle3d->GetWorldPos());

	// オブジェクトの更新
	object3d->Update(viewPro);
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
	object3d->transform.rot.y = min(object3d->transform.rot.y, rotLimit.y);
	object3d->transform.rot.x = max(object3d->transform.rot.x, -rotLimit.x);
	object3d->transform.rot.x = min(object3d->transform.rot.x, rotLimit.x);
}

void Player::Attack()
{
	if (input->GetPadButtonDown(XINPUT_GAMEPAD_A))
	{
		const float bulletSpeed = 6.0f;
		KMyMath::Vector3 bulletVec(0, 0, 1);

		const float distance = 20.0f;

		// 速度ベクトルを自機の向きに合わせて回転
		bulletVec = MyMathUtility::TransforNormal(bulletVec, object3d->transform.matWorld);

		bulletVec = MyMathUtility::MakeNormalize(bulletVec);

		// 弾発射
		BulletManager::GetInstance()->PlayerBulletShot(GetWorldPos() + bulletVec * distance, bulletVec, object3d->transform.rot, bulletSpeed);
	}
}

void Player::DeadEffect()
{
	if (!isFallEffectEnd)
	{
		// 姿勢制御
		object3d->transform.rot.x = 25.0f;
		object3d->transform.rot.y = 0;

		// 回転
		object3d->transform.rot.z += 10.0f;

		// 落下
		object3d->transform.pos.y -= 0.1f;

		// 時間経過
		fallEffectTimer++;

		// 
		expTimer++;

		if (expTimer >= max(expTimer, expTime))
		{
			ParticleManager::GetInstance()->CallSmallExp({ GetWorldPos().x + MyMathUtility::GetRand(-1.0f,1.0f),
			GetWorldPos().y + MyMathUtility::GetRand(-1.0f,1.0f),GetWorldPos().z + MyMathUtility::GetRand(-1.0f,1.0f) });
			expTimer = 0;
		}

		// 演出終わり
		if (fallEffectTimer >= max(fallEffectTimer, fallEffectTime))
		{
			ParticleManager::GetInstance()->CallExp(GetWorldPos());
			isFallEffectEnd = true;
		}
	}
}

void Player::ObjDraw()
{
	if (!isFallEffectEnd)
	{
		object3d->Draw();
	}
}

void Player::SpriteDraw()
{
	if (!isDead)
	{
		reticle2d->Draw();
	}
}

void Player::UIDraw()
{
	HPBarUI->Draw(hpbarTex, { 10,10 }, { 1,1 }, 0, { 1,1,1,1 }, false, false, { 0,0 });

	HPUI->Draw(hpTex, { 11,11 }, { HP * (318 / maxHP),30 }, 0, { 0,1,0,1 }, false, false, { 0,0 });
}

void Player::SetParent(const WorldTransfom* parent)
{
	object3d->transform.parent = parent;
}

const KMyMath::Vector3& Player::GetPosition() const
{
	return object3d->transform.pos;
}

const KMyMath::Vector3 Player::GetWorldPos() const
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}

const KMyMath::Vector3 Player::GetRot() const
{
	return object3d->transform.rot;
}

const bool Player::GetIsDead() const
{
	return isDead;
}

const bool Player::GetIsFallEffectEnd() const
{
	return isFallEffectEnd;
}

void Player::OnCollision()
{
	ParticleManager::GetInstance()->CallExp(GetWorldPos());
	HP--;
}
