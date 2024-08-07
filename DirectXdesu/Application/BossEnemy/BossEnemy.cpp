/**
 * @file BossEnemy.cpp
 * @brief ボスの基底
 * @author 飯島 薫
 */

#include "BossEnemy.h"
#include "Collision.h"
#include "Ease.h"
#include "ParticleManager.h"

void BossEnemy::Init(KGPlin* pipeline_, const KMyMath::Vector3& initPos_, KGPlin* spritePipeline_) {
	SetCollisionAttribute(Collider::Attribute::Bosss);
	SetCollisionMask((uint32_t)~Collider::Attribute::EnemysBullet);

	// パイプライン生成
	pipeline = pipeline_;
	spritePipeline = spritePipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->GetTransform().SetPos(initPos_);

	// 体力セット
	maxHP_ = 0;
	HP = maxHP_;

	HPUI_.reset(Sprite::Create(spritePipeline));

	HPrectUI_.reset(Sprite::Create(spritePipeline));

	hpTex_ = TextureManager::Load("Resources/texture/white1x1.png");

	HPBarUI_.reset(Sprite::Create(spritePipeline));

	hpbarTex_ = TextureManager::Load("Resources/texture/BossHPBar3.png");

	isHPE = true;

	isDead_ = false;

	HPPos_ = {204.0f, 40.0f};

	audioManager = AudioManager::GetInstance();
}

void BossEnemy::Draw() { object3d->Draw(); }

void BossEnemy::UIDraw() {
	if (isBossMovie_ || isDead_) {
		return;
	}

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());

	const KMyMath::Vector2 hpTexSize = {672.0f, 22.0f};

	const float sizeX = hpTexSize.x / maxHP_;
	const float sizeY = hpTexSize.y;

	HPBarUI_->Draw(hpbarTex_, {width / 2.0f, HPPos_.y}, {1.0f, 1.0f}, 0, {1.0f, 1.0f, 1.0f, 1.0f});

	const KMyMath::Vector2 HPUIPos = {(width / 2) - (hpTexSize.x / 2), HPPos_.y};

	HPrectUI_->Draw(
	    hpTex_, HPUIPos, {oldHP_ * sizeX, sizeY}, 0, {1, 0, 0, 0.3f}, false, false, {0, 0.5f});

	HPUI_->Draw(hpTex_, HPUIPos, {HP * sizeX, sizeY}, 0, {1, 0, 0, 1}, false, false, {0, 0.5f});
}

const KMyMath::Vector3 BossEnemy::GetWorldPos() const {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = object3d->GetTransform().GetWorldPos();

	return result;
}

void BossEnemy::OnCollision(Collider* collider) {
	if (isMuteki_) {
		return;
	}
	Collider* partner = collider;

	// ボム
	if (partner->GetCollisionAttribute() == Collider::Attribute::PlayersBom) {
		HP -= 50;
	}
	// 弾
	else if (partner->GetCollisionAttribute() == Collider::Attribute::PlayersBullet) {
		HP -= 5;
	}

	ObjParticleManager::GetInstance()->SetExp(GetWorldPos());
	hpEase = true;
	oldHpTimer = 0;
	hpEaseTimer = 0;
}

KMyMath::Vector3 BossEnemy::GetWorldPosition() {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = object3d->GetTransform().GetWorldPos();

	return result;
}

bool BossEnemy::CollisionCheck(const KMyMath::Vector3& posA_, const KMyMath::Vector3& posB_) {
	if (MyCollisions::CheckSphereToSphere(posA_, posB_, 1, 1)) {
		return true;
	}

	return false;
}

void BossEnemy::DeadEffect() {
	if (!isFallEffectEnd) {
		// 姿勢制御
		object3d->GetTransform().SetRot({0, 0, object3d->GetTransform().GetRot().z});

		// 回転
		object3d->GetTransform().SetRot(MyEase::Lerp3D(
		    {0.0f, 0.0f, 0.0f}, {20.0f, 0.0f, 20.0f}, fallEffectTimer / fallEffectTime));

		// 落下
		object3d->GetTransform().AddSetPos({0.0f, -0.15f, 0.0f});

		// 時間経過
		fallEffectTimer++;

		//
		expTimer++;

		if (expTimer >= max(expTimer, expTime)) {
			audioManager->SEPlay_wav("syouBakuhatuSE.wav", 0.15f);
			ObjParticleManager::GetInstance()->SetSmallExp(
			    {GetWorldPos().x + MyMathUtility::GetRandF(-10.0f, 10.0f),
			     GetWorldPos().y + MyMathUtility::GetRandF(-10.0f, 10.0f),
			     GetWorldPos().z + MyMathUtility::GetRandF(-10.0f, 10.0f)});
			expTimer = 0;
		}

		// 演出終わり
		if (fallEffectTimer >= max(fallEffectTimer, fallEffectTime)) {
			audioManager->SEPlay_wav("bakuhatuSE.wav", 0.5f);
			ObjParticleManager::GetInstance()->SetExp(GetWorldPos());
			isFallEffectEnd = true;
		}
	}
}

void BossEnemy::HPEffect() {
	if (hpEase) {
		if (oldHpTimer < oldHpTime) {
			oldHpTimer++;
		} else {
			hpEaseTimer++;

			oldHP_ = MyEase::OutCubicFloat(startHpEase, HP, hpEaseTimer / hpEaseTime);

			if (hpEaseTimer > hpEaseTime) {
				hpEase = false;
			}
		}
	} else {
		oldHP_ = HP;
		startHpEase = oldHP_;
		oldHpTimer = 0;
		hpEaseTimer = 0;
	}
}

const bool BossEnemy::GetIsHPE() const { return isHPE; }

const bool BossEnemy::GetIsDead() const { return isDead_; }

const bool BossEnemy::GetIsFallEffectEnd() const { return isFallEffectEnd; }

const KMyMath::Vector3& BossEnemy::GetRot() const { return object3d->GetTransform().GetRot(); }

void BossEnemy::SetPos(const KMyMath::Vector3& pos_) { object3d->GetTransform().SetPos(pos_); }

void BossEnemy::SetRot(const KMyMath::Vector3& rot_) { object3d->GetTransform().SetRot(rot_); }

void BossEnemy::SetIsMuteki(bool isMuteki) { isMuteki_ = isMuteki; }
