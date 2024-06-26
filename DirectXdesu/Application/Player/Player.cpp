#include "Player.h"
#include "BulletManager.h"
#include "Ease.h"
#include "ImguiManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "PipelineManager.h"
#include "ScoreManager.h"

#include "PostEffectManager.h"
#include "VignettePostEffect.h"

#include "GameManager.h"

const float Player::moveSpeed = 0.48f;
const KMyMath::Vector2 Player::rotLimit = {35.0f, 25.0f};
const KMyMath::Vector2 Player::posLimitMin = {-15.0f, -4.0f};
const KMyMath::Vector2 Player::posLimitMax = {15.0f, Player::posLimitMin.y + 12.0f};

Player* Player::nowPlayer = nullptr;

bool Player::isStartEase = false;

Player* Player::Create(const float playerHP) {
	// インスタンス生成
	Player* player = new Player();
	if (player == nullptr) {
		return nullptr;
	}

	// 初期化
	player->Init(playerHP);

	return player;
}

void Player::Init(const float playerHP) {
	SetCollisionAttribute(Collider::Attribute::Players);
	SetCollisionMask((uint32_t)~Collider::Attribute::PlayersBullet);

	// 入力インスタンス
	input = KInput::GetInstance();

	// オブジェクト生成
	object3d.reset(KObject3d::Create(
	    ModelManager::GetInstance()->GetModels("Player"),
	    PipelineManager::GetInstance()->GetPipeline("Obj")));
	object3d->GetTransform().SetPos({0, 0, 50});
	object3d->GetTransform().SetScale({2.0f, 2.0f, 2.0f});

	// レティクル
	reticle3d = std::make_unique<Reticle3D>();
	reticle3d->Init();
	reticle2d = std::make_unique<Reticle2D>();
	reticle2d->Init();

	// HP関連
	maxHP = playerHP;
	HP = maxHP;

	// HPバー
	HPUI.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	// HP減少値バー
	HPrectUI.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	// HPテクスチャ読み込み
	hpTex = TextureManager::GetInstance()->GetTextures("White1x1");

	// HPゲージ
	HPBarUI.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	// HPゲージテクスチャ読み込み
	hpbarTex = TextureManager::Load("Resources/texture/PlayerHPBar2.png");

	// 死亡フラグ
	isDead = false;

	// ダメージ演出フラグ
	isDamageEffect = false;

	// ピンチフラグ
	isCrisis = false;

	// ダメージエフェクト
	damageEffect.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	// ダメージエフェクトテクスチャ読み込み
	damageTex = TextureManager::Load("Resources/texture/damage.png");

	// ダメージエフェクトの透過値
	dAlpha = 0;

	// 操作説明
	operation.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	operationTex = TextureManager::Load("Resources/texture/setumei.png");
	operationPos = {
	    KWinApp::GetInstance()->GetWindowSizeW() + 450.0f,
	    KWinApp::GetInstance()->GetWindowSizeH() + 100.0f};

	// ボムアイコン
	bomIcon.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	bomIconTex = TextureManager::Load("Resources/texture/BomIcon.png");

	// ボムクールタイム
	bomCoolIcon.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	audioManager = AudioManager::GetInstance();
}

void Player::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	ImGui::Begin("Player");
	ImGui::SetWindowPos({400, 10});
	ImGui::SetWindowSize({200, 200});
	if (ImGui::Button("MUTEKI_Mode")) {
		if (muteki) {
			muteki = false;
		} else {
			muteki = true;
		}
	}
	ImGui::SliderFloat(
	    "HPPosX", &HPPos.x, 0, (float)KWinApp::GetInstance()->GetWindowSizeW() * 1 / 3, "%.1f");
	ImGui::SliderFloat(
	    "HPPosY", &HPPos.y, (float)KWinApp::GetInstance()->GetWindowSizeH() * 2 / 3,
	    (float)KWinApp::GetInstance()->GetWindowSizeH(), "%.1f");
	ImGui::SliderFloat("HPSizeX", &HPSize.x, 0, 1, "%.1f");
	ImGui::SliderFloat("HPSizeY", &HPSize.y, 0, 1, "%.1f");

	ImGui::SliderFloat(
	    "BomIconX", &bomIconPos.x, 0, (float)KWinApp::GetInstance()->GetWindowSizeW() * 1 / 3,
	    "%.1f");
	ImGui::SliderFloat(
	    "BomIconY", &bomIconPos.y, (float)KWinApp::GetInstance()->GetWindowSizeH() * 2 / 3,
	    (float)KWinApp::GetInstance()->GetWindowSizeH(), "%.1f");
	ImGui::End();

	SudCoolTime();

	BomsCoolTime();

	// スタート演出中の処理
	if (GameManager::GetInstance()->GetMovieFlag("All")) {

	}
	// スタートイージング
	else if (isStartEase) {
		StandStartPos();
	}
	// ゲーム中
	else {
		// 死亡条件
		if (HP <= min(HP, 0)) {
			HP = 0;
			isDead = true;
		}

		if (!isDead) {
			// 移動
			Move();

			// 回転
			Rot();

			// 攻撃
			Attack();

#ifdef _DEBUG
			Debug();
#endif // _DEBUG
		} else {
			// 死亡演出
			DeadEffect();
		}

		// HP演出
		HPEffect();

		// ダメージ演出
		DamageEffect();
	}

	hpShake.Update();

	// 3Dレティクルの更新
	KMyMath::Matrix4 nowMatWorld = object3d->GetTransform().GetMatWorld();
	reticle3d->Update(nowMatWorld, GetWorldPos());

	// 2Dレティクルの更新
	reticle2d->Update(viewPro, reticle3d->GetWorldPos());

	// オブジェクトの更新
	object3d->Update(viewPro, cameraPos);
}

void Player::Move() {
	// 自機が傾いている角度に移動させる
	KMyMath::Vector3 velocity = {0, 0, 0};
	velocity.x = (object3d->GetTransform().GetRot().y / rotLimit.y);
	velocity.y = -(object3d->GetTransform().GetRot().x / rotLimit.x);

	// 動け～
	object3d->GetTransform().AddSetPos({velocity.x * moveSpeed, velocity.y * moveSpeed, 0});

	// 移動制限
	object3d->GetTransform().SetPos(
	    {max(object3d->GetTransform().GetPos().x, posLimitMin.x),
	     max(object3d->GetTransform().GetPos().y, posLimitMin.y),
	     object3d->GetTransform().GetPos().z});
	object3d->GetTransform().SetPos(
	    {min(object3d->GetTransform().GetPos().x, posLimitMax.x),
	     min(object3d->GetTransform().GetPos().y, posLimitMax.y),
	     object3d->GetTransform().GetPos().z});
}

void Player::Rot() {
	// 回転速度
	const float rotSpeed = 0.04f;

	// 角度修正基準速度
	const float correctionSpeed = (rotSpeed * 2.5f) / 0.1f;
	KMyMath::Vector3 rot = {0, 0, 0};

	// どこまで傾けたら判定をとるか
	const float stickNum = 100;

	// Y軸回転
	if (input->LStickTiltX(stickNum) || input->LStickTiltX(-stickNum)) {
		// 自機はスティックを倒した方向に動く
		const float stickRota = input->GetLStickAngle();
		const float moveAngle = DirectX::XMConvertToRadians(stickRota);
		const float padStickIncline = input->GetLStickInline().x;
		rot.y = rotSpeed * cosf(moveAngle) * fabsf(padStickIncline);
	} else {
		// 角度修正速度倍率
		float backSpeedRatio = fabsf(object3d->GetTransform().GetRot().y / (rotLimit.y * 2)) + 0.5f;
		// 角度修正速度
		const float backSpeed = correctionSpeed * backSpeedRatio;
		// y軸回転の傾きを修正する
		const float rotMin = 0.5f;
		if (object3d->GetTransform().GetRot().y > rotMin) {
			rot.y -= backSpeed;
		} else if (object3d->GetTransform().GetRot().y < -rotMin) {
			rot.y += backSpeed;
		} else {
			object3d->GetTransform().SetRot(
			    {object3d->GetTransform().GetRot().x, 0, object3d->GetTransform().GetRot().z});
		}
	}

	// X軸回転
	if (input->LStickTiltY(stickNum) || input->LStickTiltY(-stickNum)) {
		// 自機はスティックを倒した方向に動く
		const float stickRota = -input->GetLStickAngle();
		const float moveAngle = DirectX::XMConvertToRadians(stickRota);
		const float padStickIncline = input->GetLStickInline().y;
		rot.x = rotSpeed * sinf(moveAngle) * fabsf(padStickIncline);
	} else {
		// 角度修正速度倍率
		float backSpeedRatio = fabsf(object3d->GetTransform().GetRot().x / (rotLimit.x * 2)) + 0.5f;
		// 角度修正速度
		const float backSpeed = correctionSpeed * backSpeedRatio;
		// y軸回転の傾きを修正する
		const float rotMin = 0.5f;
		if (object3d->GetTransform().GetRot().x > rotMin) {
			rot.x -= backSpeed;
		} else if (object3d->GetTransform().GetRot().x < -rotMin) {
			rot.x += backSpeed;
		} else {
			object3d->GetTransform().SetRot(
			    {0, object3d->GetTransform().GetRot().y, object3d->GetTransform().GetRot().z});
		}
	}

	// Z軸回転
	{
		const float rotZspeed = 0.01f;
		const float rotZLimit = 1.0f;
		// 右回転
		if (isRotZRight) {
			swayZ += rotZspeed;
			if (swayZ >= rotZLimit) {
				isRotZRight = false;
			}
		}
		// 左回転
		else {
			swayZ -= rotZspeed;
			if (swayZ <= -rotZLimit) {
				isRotZRight = true;
			}
		}

		object3d->GetTransform().SetRot(
		    {object3d->GetTransform().GetRot().x, object3d->GetTransform().GetRot().y,
		     -object3d->GetTransform().GetRot().y + swayZ});
	}

	object3d->GetTransform().AddSetRot(rot);

	// 角度制限
	object3d->GetTransform().SetRot(
	    {max(object3d->GetTransform().GetRot().x, -rotLimit.x),
	     max(object3d->GetTransform().GetRot().y, -rotLimit.y),
	     object3d->GetTransform().GetRot().z});
	object3d->GetTransform().SetRot(
	    {min(object3d->GetTransform().GetRot().x, rotLimit.x),
	     min(object3d->GetTransform().GetRot().y, rotLimit.y),
	     object3d->GetTransform().GetRot().z});
}

void Player::Attack() {
	if (input->GetPadButton(XINPUT_GAMEPAD_A) && coolTimer <= 0) {
		// 弾スピード
		const float bulletSpeed = 9.0f;
		KMyMath::Vector3 bulletVec(0, 0, 1);

		// 自機と発射位置の距離
		const float distance = 20.0f;

		const float bulletPower = 5.0f;

		// 速度ベクトルを自機の向きに合わせて回転
		bulletVec =
		    MyMathUtility::TransforNormal(bulletVec, object3d->GetTransform().GetMatWorld());

		// 正規化
		bulletVec = MyMathUtility::MakeNormalize(bulletVec);

		// 弾発射
		BulletManager::GetInstance()->PlayerBulletShot(
		    GetWorldPos() + (bulletVec * distance), // ポジション＋(角度＊距離)
		    bulletVec,                              // 弾の進む向き
		    object3d->GetTransform().GetRot(),      // 角度取得
		    bulletSpeed,                            // 弾の速度
		    bulletPower                             // 弾のパワー
		);

		// SE鳴らし
		audioManager->SEPlay_wav("shotSE.wav");

		// クールタイムセット
		coolTimer = coolTimeSet;
	}

	if (input->GetPadButtonDown(XINPUT_GAMEPAD_B) && isBom) {
		audioManager->SEPlay_wav("bomShot.wav");

		// 弾スピード
		const float bulletSpeed = 3.0f;
		KMyMath::Vector3 bulletVec(0, 0, 1);

		// 自機と発射位置の距離
		const float distance = 20.0f;

		// 速度ベクトルを自機の向きに合わせて回転
		bulletVec =
		    MyMathUtility::TransforNormal(bulletVec, object3d->GetTransform().GetMatWorld());

		// 正規化
		bulletVec = MyMathUtility::MakeNormalize(bulletVec);

		// 弾発射
		BulletManager::GetInstance()->BomShot(
		    GetWorldPos() + (bulletVec * distance), // ポジション＋(角度＊距離)
		    bulletVec,                              // 弾の進む向き
		    object3d->GetTransform().GetRot(),      // 角度取得
		    bulletSpeed                             // 弾の速度
		);

		bomsCoolTimer = bomsCoolTime;
	}
}

void Player::DeadEffect() {
	if (!isFallEffectEnd) {
		// 姿勢制御
		object3d->GetTransform().SetRot({25.0f, 0.0f, object3d->GetTransform().GetRot().z});

		// 回転
		object3d->GetTransform().AddSetRot({0.0f, 0.0f, 10.0f});

		// 落下
		object3d->GetTransform().AddSetPos({0.0f, -0.1f, 0.5f});

		// 時間経過
		fallEffectTimer++;

		// 爆発間隔タイマー
		expTimer++;

		// 堕ちてる間の爆発
		if (expTimer >= max(expTimer, expTime)) {
			audioManager->SEPlay_wav("syouBakuhatuSE.wav", 0.15f);
			ObjParticleManager::GetInstance()->SetSmallExp(
			    {GetWorldPos().x + MyMathUtility::GetRandF(-1.0f, 1.0f),
			     GetWorldPos().y + MyMathUtility::GetRandF(-1.0f, 1.0f),
			     GetWorldPos().z + MyMathUtility::GetRandF(-1.0f, 1.0f)});
			expTimer = 0;
		}

		// 演出終わり
		if (fallEffectTimer >= max(fallEffectTimer, fallEffectTime)) {
			audioManager->SEPlay_wav("bakuhatuSE.wav");
			ObjParticleManager::GetInstance()->SetExp(GetWorldPos());
			isFallEffectEnd = true;
		}
	}
}

void Player::HPEffect() {
	// HP減少演出処理
	if (hpEase) {
		if (oldHpTimer < oldHpTime) {
			oldHpTimer++;
		} else {
			hpEaseTimer++;

			oldHP = MyEase::OutCubicFloat(startHpEase, HP, hpEaseTimer / hpEaseTime);

			if (hpEaseTimer > hpEaseTime) {
				hpEase = false;
			}
		}
	} else {
		oldHP = HP;
		startHpEase = oldHP;
		oldHpTimer = 0;
		hpEaseTimer = 0;
	}

	// ピンチ状態のHP演出
	if (HP < maxHP * 1 / 4) {
		if (!isCrisis) {
			audioManager->SEPlay_wav("alertSE.wav");
			isCrisis = true;
		}
		hpColor = {1, 0, 0, 1};
	} else {
		isCrisis = false;
		hpColor = {0, 1, 0, 1};
	}
}

void Player::DamageEffect() {
	// ダメージエフェクト処理
	if (isDamageEffect) {
		dAlpha += 0.02f;
		VignettePostEffect::SetVignetteNum(dAlpha);

		if (dAlpha > 0.8f) {
			isDamageEffect = false;
			PostEffectManager::SetPostMode(PostEffectManager::Normal);
		}
	}

	// 無敵時間処理
	if (isInvisible) {
		if (invisibleTimer < invisibleTime) {
			invisibleTimer++;
			if (invisibleTimer % 10 == 0) {
				object3d->SetAlpha(255.0f);
			} else {
				object3d->SetAlpha(0.50f);
			}
		} else {
			invisibleTimer = 0;
			isInvisible = false;
		}
	}
}

void Player::SudCoolTime() {
	if (coolTimer > 0) {
		coolTimer--;
	}
}

void Player::EndStart() { object3d->GetTransform().SetPos({0, 0, 50}); }

void Player::StandStartPos() {
	startEaseTime = 30;

	if (startEaseTimer < startEaseTime) {
		startEaseTimer++;

		float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
		float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

		HPPos = MyEase::OutCubicVec2(
		    {-600.0f, height + 200.0f}, {32.0f, height - 32.0f}, startEaseTimer / startEaseTime);

		bomIconPos = MyEase::OutCubicVec2(
		    {180.0f, height + 200.0f}, {360.0f, 648.5f}, startEaseTimer / startEaseTime);

		operationPos = MyEase::OutCubicVec2(
		    {width + 450.0f, height + 100.0f}, {width, height}, startEaseTimer / startEaseTime);

		object3d->GetTransform().SetPos(
		    {object3d->GetTransform().GetPos().x, object3d->GetTransform().GetPos().y,
		     MyEase::OutCubicFloat(-50.0f, 50.0f, startEaseTimer / startEaseTime)});

		object3d->GetTransform().SetRot(
		    {0.0f, 0.0f, MyEase::InOutCubicFloat(0.0f, 360.0f, startEaseTimer / startEaseTime)});
	} else {
		isStartEase = false;
	}
}

void Player::BomsCoolTime() {
	if (bomsCoolTimer >= 1) {
		bomsCoolTimer--;
		bomIconAlpha = 0.5f;
		isBom = false;
	} else {
		bomIconAlpha = 1.0f;
		isBom = true;
	}
}

void Player::Debug() {}

void Player::ObjDraw() {
	// 機体描画
	if (!isFallEffectEnd) {
		object3d->Draw();
	}
}

void Player::SpriteDraw() { reticle2d->Draw(); }

void Player::UIDraw() {
	KMyMath::Vector2 HPsize = {286 / maxHP, 17};

	// HPバー描画
	HPBarUI->Draw(
	    hpbarTex, (HPPos + HPBarUIPos) + hpShake.GetShakePos(), HPSize, 0, {1, 1, 1, 1}, false,
	    false, {0, 1});

	// HP減少値描画
	HPrectUI->Draw(
	    hpTex, (HPPos + HPUIPos) + hpShake.GetShakePos(), {oldHP * HPsize.x, HPsize.y}, 0,
	    {hpColor.x, hpColor.y, hpColor.z, 0.3f}, false, false, {0, 1});

	// HP描画
	HPUI->Draw(
	    hpTex, (HPPos + HPUIPos) + hpShake.GetShakePos(), {HP * HPsize.x, HPsize.y}, 0,
	    {hpColor.x, hpColor.y, hpColor.z, 1}, false, false, {0, 1});

	// ボムアイコン描画
	bomIcon->Draw(
	    bomIconTex, bomIconPos, {0.1f, 0.1f}, 0.0f, {1, 1, 1, bomIconAlpha}, false, false, {0, 0});
	if (!isBom) {
		bomCoolIcon->Draw(
		    hpTex, bomIconPos, {50.0f, bomsCoolTimer * (50.0f / bomsCoolTime)}, 0.0f,
		    {0, 0, 0, 0.8f}, false, false, {0, 0});
	}

	// 操作説明
	operation->Draw(
	    operationTex, operationPos, {1.0f, 1.0f}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, false, false,
	    {1.0f, 1.0f});
}

void Player::SetParent(const Transform* parent_) { object3d->SetParent(parent_); }

const KMyMath::Vector3& Player::GetPosition() const { return object3d->GetTransform().GetPos(); }

const KMyMath::Vector3 Player::GetWorldPos() const {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = object3d->GetTransform().GetWorldPos();

	return result;
}

const KMyMath::Vector3 Player::GetRot() const { return object3d->GetTransform().GetRot(); }

const bool Player::GetIsDead() const { return isDead; }

const bool Player::GetIsFallEffectEnd() const { return isFallEffectEnd; }

void Player::SetPos(const KMyMath::Vector3& pos_) { object3d->GetTransform().SetPos(pos_); }

void Player::SetRot(const KMyMath::Vector3& rot_) { object3d->GetTransform().SetRot(rot_); }

void Player::SetScale(const KMyMath::Vector3& scale_) { object3d->GetTransform().SetScale(scale_); }

const KMyMath::Vector2& Player::GetRotLimit() { return rotLimit; }

const float Player::GetMoveSpeed() { return moveSpeed; }

const KMyMath::Vector2& Player::GetPosLimitMax() { return posLimitMax; }

const KMyMath::Vector2& Player::GetPosLimitMin() { return posLimitMin; }

const bool Player::GetIsInvisible() const { return isInvisible; }

void Player::OnCollision(Collider* collider) {
	Collider* partner = collider;
	audioManager->SEPlay_wav("damageSE.wav", 0.15f);
	ObjParticleManager::GetInstance()->SetSmallExp(GetWorldPos());

	float damage = 0;

	// ボスとの判定
	if (partner->GetCollisionAttribute() == Collider::Attribute::Bosss) {
		damage = 5;
	}
	// 弾の判定
	else if (partner->GetCollisionAttribute() == Collider::Attribute::EnemysBullet) {
		damage = 2;
	}
	// それ以外(レーザー)
	else {
		damage = 5;
	}

	if (!muteki) {
		HP -= damage;
	}

	hpEase = true;
	oldHpTimer = 0;
	hpEaseTimer = 0;
	isDamageEffect = true;
	dAlpha = 0.1f;
	VignettePostEffect::SetVignetteNum(dAlpha);
	PostEffectManager::SetPostMode(PostEffectManager::Vignette);
	isInvisible = true;
	hpShake.SetShake(5, -5, 30.0f);
	ScoreManager::GetInstance()->AddDamageCount();
}

KMyMath::Vector3 Player::GetWorldPosition() {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = object3d->GetTransform().GetWorldPos();

	return result;
}
