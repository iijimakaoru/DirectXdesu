#include "Player.h"
#include "Ease.h"
#include "BulletManager.h"
#include "ParticleManager.h"

const float Player::moveSpeed = 0.48f;
const KMyMath::Vector2 Player::rotLimit = { 35.0f, 25.0f };
const KMyMath::Vector2 Player::posLimitMin = { -15.0f, -4.0f };
const KMyMath::Vector2 Player::posLimitMax = { 15.0f, Player::posLimitMin.y + 12.0f };

bool Player::isStartEase = false;

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
	// 入力インスタンス
	input = KInput::GetInstance();

	// モデル生成
	model = model_;

	// パイプライン生成
	objPipeline = objPipeline_;
	spritePipeline = spritePipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, objPipeline));
	object3d->SetPos({ 0,0,50 });
	object3d->SetScale({ 2.0f,2.0f,2.0f });

	// レティクル
	reticle3d = std::make_unique<Reticle3D>();
	reticle3d->Init();
	reticle2d = std::make_unique<Reticle2D>();
	reticle2d->Init();

	// HP関連
	maxHP = playerHP;
	HP = maxHP;

	// HPバー
	HPUI = std::make_unique<Sprite>();
	HPUI->Init();
	HPUI->SetPipeline(spritePipeline);

	// HP減少値バー
	HPrectUI = std::make_unique<Sprite>();
	HPrectUI->Init();
	HPrectUI->SetPipeline(spritePipeline);

	// HPテクスチャ読み込み
	hpTex = TextureManager::Load("Resources/texture/white1x1.png");

	// HPゲージ
	HPBarUI = std::make_unique<Sprite>();
	HPBarUI->Init();
	HPBarUI->SetPipeline(spritePipeline);

	// HPゲージテクスチャ読み込み
	hpbarTex = TextureManager::Load("Resources/texture/PlayersHPBar.png");

	// 死亡フラグ
	isDead = false;

	// ダメージ演出フラグ
	isDamageEffect = false;

	// ダメージエフェクト
	damage = std::make_unique<Sprite>();
	damage->Init();
	damage->SetPipeline(spritePipeline);

	// ダメージエフェクトテクスチャ読み込み
	damageTex = TextureManager::Load("Resources/texture/damage.png");

	// ダメージエフェクトの透過値
	dAlpha = 0;

	isStart = true;

	isStartEase = true;
}

void Player::Update(ViewProjection* viewPro)
{
	// スタート演出中の処理
	if (isStart)
	{
		StartEffect();
	}
	else if (isStartEase)
	{
		StandStartPos();
	}
	else
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

#ifdef _DEBUG
			Debug();
#endif // _DEBUG
		}
		else
		{
			// 死亡演出
			DeadEffect();
		}

		// HP演出
		HPEffect();

		// ダメージ演出
		DamageEffect();
	}

	// 3Dレティクルの更新
	KMyMath::Matrix4 nowMatWorld = object3d->GetMatWorld();
	reticle3d->Update(nowMatWorld, GetWorldPos());

	// 2Dレティクルの更新
	reticle2d->Update(viewPro, reticle3d->GetWorldPos());

	// オブジェクトの更新
	object3d->Update(viewPro);
}

void Player::SetPos(const KMyMath::Vector3 pos_)
{
	object3d->SetPos(pos_);
}

void Player::Move()
{
	//自機が傾いている角度に移動させる
	KMyMath::Vector3 velocity = { 0, 0, 0 };
	velocity.x = (object3d->GetRot().y / rotLimit.y);
	velocity.y = -(object3d->GetRot().x / rotLimit.x);

	// 動け～
	object3d->AddSetPos({ velocity.x * moveSpeed ,velocity.y * moveSpeed ,0 });

	// 移動制限
	object3d->SetPos({ max(object3d->GetPos().x, posLimitMin.x) ,
		max(object3d->GetPos().y, posLimitMin.y) ,
		object3d->GetPos().z });
	object3d->SetPos({ min(object3d->GetPos().x, posLimitMax.x) ,
		min(object3d->GetPos().y, posLimitMax.y) ,
		object3d->GetPos().z });
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
		const float moveAngle = DirectX::XMConvertToRadians(stickRota);
		const float padStickIncline = input->GetLStickInline().x;
		rot.y = rotSpeed * cosf(moveAngle) * fabsf(padStickIncline);
	}
	else
	{
		//角度修正速度倍率
		float backSpeedRatio = fabsf(object3d->GetRot().y / (rotLimit.y * 2)) + 0.5f;
		//角度修正速度
		const float backSpeed = correctionSpeed * backSpeedRatio;
		//y軸回転の傾きを修正する
		const float rotMin = 0.5f;
		if (object3d->GetRot().y > rotMin)
		{
			rot.y -= backSpeed;
		}
		else if (object3d->GetRot().y < -rotMin)
		{
			rot.y += backSpeed;
		}
		else
		{
			object3d->SetRot({ object3d->GetRot().x,0,object3d->GetRot().z });
		}
	}

	// X軸回転
	if (input->LStickTiltY(stickNum) || input->LStickTiltY(-stickNum))
	{
		//自機はスティックを倒した方向に動く
		const float stickRota = -input->GetLStickAngle();
		const float moveAngle = DirectX::XMConvertToRadians(stickRota);
		const float padStickIncline = input->GetLStickInline().y;
		rot.x = rotSpeed * sinf(moveAngle) * fabsf(padStickIncline);
	}
	else
	{
		//角度修正速度倍率
		float backSpeedRatio = fabsf(object3d->GetRot().x / (rotLimit.x * 2)) + 0.5f;
		//角度修正速度
		const float backSpeed = correctionSpeed * backSpeedRatio;
		//y軸回転の傾きを修正する
		const float rotMin = 0.5f;
		if (object3d->GetRot().x > rotMin)
		{
			rot.x -= backSpeed;
		}
		else if (object3d->GetRot().x < -rotMin)
		{
			rot.x += backSpeed;
		}
		else
		{
			object3d->SetRot({ 0,object3d->GetRot().y,object3d->GetRot().z });
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

		object3d->SetRot({ object3d->GetRot().x,object3d->GetRot().y,-object3d->GetRot().y + swayZ});
	}

	object3d->AddSetRot(rot);

	// 角度制限
	object3d->SetRot({ max(object3d->GetRot().x, -rotLimit.x),
		max(object3d->GetRot().y, -rotLimit.y) ,
		object3d->GetRot().z });
	object3d->SetRot({ min(object3d->GetRot().x, rotLimit.x),
		min(object3d->GetRot().y, rotLimit.y),
		object3d->GetRot().z });
}

void Player::Attack()
{
	if (input->GetPadButtonDown(XINPUT_GAMEPAD_A))
	{
		const float bulletSpeed = 6.0f;
		KMyMath::Vector3 bulletVec(0, 0, 1);

		const float distance = 20.0f;

		// 速度ベクトルを自機の向きに合わせて回転
		bulletVec = MyMathUtility::TransforNormal(bulletVec, object3d->GetMatWorld());

		bulletVec = MyMathUtility::MakeNormalize(bulletVec);

		// 弾発射
		BulletManager::GetInstance()->PlayerBulletShot(GetWorldPos() + bulletVec * distance,
			bulletVec,
			object3d->GetRot(),
			bulletSpeed);
	}
}

void Player::DeadEffect()
{
	if (!isFallEffectEnd)
	{
		// 姿勢制御
		object3d->SetRot({ 25.0f,0.0f,object3d->GetRot().z });

		// 回転
		object3d->AddSetRot({ 0.0f,0.0f,10.0f });

		// 落下
		object3d->AddSetPos({ 0.0f,-0.1f,0.5f });

		// 時間経過
		fallEffectTimer++;

		// 爆発間隔タイマー
		expTimer++;

		// 堕ちてる間の爆発
		if (expTimer >= max(expTimer, expTime))
		{
			ObjParticleManager::GetInstance()->SetSmallExp({ GetWorldPos().x + MyMathUtility::GetRand(-1.0f,1.0f),
			 GetWorldPos().y + MyMathUtility::GetRand(-1.0f,1.0f), GetWorldPos().z + MyMathUtility::GetRand(-1.0f,1.0f) });
			expTimer = 0;
		}

		// 演出終わり
		if (fallEffectTimer >= max(fallEffectTimer, fallEffectTime))
		{
			ObjParticleManager::GetInstance()->SetExp(GetWorldPos());
			isFallEffectEnd = true;
		}
	}
}

void Player::HPEffect()
{
	// HP減少演出処理
	if (hpEase)
	{
		if (oldHpTimer < oldHpTime)
		{
			oldHpTimer++;
		}
		else
		{
			hpEaseTimer++;

			oldHP = MyEase::OutCubicFloat(startHpEase, HP, hpEaseTimer / hpEaseTime);

			if (hpEaseTimer > hpEaseTime)
			{
				hpEase = false;
			}
		}
	}
	else
	{
		oldHP = HP;
		startHpEase = oldHP;
		oldHpTimer = 0;
		hpEaseTimer = 0;
	}

	// ピンチ状態のHP演出
	if (HP < maxHP * 1 / 4)
	{
		hpColor = { 1,0,0,1 };
	}
	else
	{
		hpColor = { 0,1,0,1 };
	}
}

void Player::DamageEffect()
{
	// ダメージエフェクト処理
	if (isDamageEffect)
	{
		dAlpha -= 0.1f;

		if (dAlpha < 0)
		{
			isDamageEffect = false;
		}
	}

	// 無敵時間処理
	if (isInvisible)
	{
		if (invisibleTimer < invisibleTime)
		{
			invisibleTimer++;
		}
		else
		{
			invisibleTimer = 0;
			isInvisible = false;
		}
	}
}

void Player::StartEffect()
{
	if (startPhase == 0)
	{
		phaseTime = 660;

		if (phaseTimer < phaseTime)
		{
			phaseTimer++;
		}
		else
		{
			startPhase++;
			phaseTimer = 0;
		}
	}
	else if (startPhase == 1)
	{
		phaseTime = 60;

		if (phaseTimer < phaseTime)
		{
			phaseTimer++;

			object3d->SetPos({ object3d->GetPos().x,
				object3d->GetPos().y,
				MyEase::OutCubicFloat(50.0f,100.0f,phaseTimer / phaseTime) });
		}
		else
		{
			startPhase++;
			phaseTimer = 0;
		}
	}
	else
	{
		isStart = false;
	}
}

void Player::EndStart()
{
	object3d->SetPos({ 0,0,50 });
}

void Player::StandStartPos()
{
	startEaseTime = 30;

	if (startEaseTimer < startEaseTime)
	{
		startEaseTimer++;

		object3d->SetPos({ object3d->GetPos().x,
			object3d->GetPos().y,
			MyEase::OutCubicFloat(-50.0f,50.0f,startEaseTimer / startEaseTime) });

		object3d->SetRot({ 0.0f,
			0.0f,
			MyEase::InOutCubicFloat(0.0f,360.0f,startEaseTimer / startEaseTime) });
	}
	else
	{
		isStartEase = false;
	}
}

void Player::Debug()
{

}

void Player::ObjDraw()
{
	// 機体描画
	if (!isFallEffectEnd)
	{
		object3d->Draw();
	}
}

void Player::SpriteDraw()
{
	// レティクル描画
	if (isDead || isStart || isStartEase)
	{
		return;
	}

	reticle2d->Draw();
}

void Player::UIDraw()
{
	if (isStart || isStartEase)
	{
		return;
	}

	// HPバー描画
	HPBarUI->Draw(hpbarTex, { 10,10 }, { 1,1 }, 0, { 1,1,1,1 }, false, false, { 0,0 });

	// HP減少値描画
	HPrectUI->Draw(hpTex, { 11,11 }, { oldHP * (318 / maxHP),30 }, 0, { hpColor.x,hpColor.y,hpColor.z,0.3f }, false, false, { 0,0 });

	// HP描画
	HPUI->Draw(hpTex, { 11,11 }, { HP * (318 / maxHP),30 }, 0, { hpColor.x,hpColor.y,hpColor.z,1 }, false, false, { 0,0 });

	// ダメージエフェクト描画
	if (isDamageEffect)
	{
		damage->Draw(damageTex, { 1280 / 2, 720 / 2 }, { 1 ,1 }, 0.0f, { 1,0,0,dAlpha });
	}
}

void Player::SetParent(const WorldTransfom* parent_)
{
	object3d->SetParent(parent_);
}

const KMyMath::Vector3& Player::GetPosition() const
{
	return object3d->GetPos();
}

const KMyMath::Vector3 Player::GetWorldPos() const
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->GetMatWorld().m[3][0];
	result.y = object3d->GetMatWorld().m[3][1];
	result.z = object3d->GetMatWorld().m[3][2];

	return result;
}

const KMyMath::Vector3 Player::GetRot() const
{
	return object3d->GetRot();
}

const bool Player::GetIsDead() const
{
	return isDead;
}

const bool Player::GetIsFallEffectEnd() const
{
	return isFallEffectEnd;
}

const KMyMath::Vector2& Player::GetRotLimit()
{
	return rotLimit;
}

const float Player::GetMoveSpeed()
{
	return moveSpeed;
}

const KMyMath::Vector2& Player::GetPosLimitMax()
{
	return posLimitMax;
}

const KMyMath::Vector2& Player::GetPosLimitMin()
{
	return posLimitMin;
}

const bool Player::GetIsInvisible() const
{
	return isInvisible;
}

const bool Player::GetIsStart() const
{
	return isStart;
}

void Player::OnCollision()
{
	ObjParticleManager::GetInstance()->SetSmallExp(GetWorldPos());
	HP--;
	hpEase = true;
	oldHpTimer = 0;
	hpEaseTimer = 0;
	isDamageEffect = true;
	dAlpha = 1;
	isInvisible = true;
}
