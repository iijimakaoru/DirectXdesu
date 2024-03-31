#include "Explosion.h"
#include "Ease.h"
#include "ModelManager.h"
#include "PipelineManager.h"

Explosion* Explosion::Create(const KMyMath::Vector3& pos, const float time) {
	// インスタンス
	Explosion* instance = new Explosion();
	if (instance == nullptr) {
		return nullptr;
	}

	instance->Init();
	instance->Set(pos, time);

	return instance;
}

void Explosion::Init() {
	SetCollisionAttribute(Collider::Attribute::PlayersBullet);
	SetCollisionMask((uint32_t) ~Collider::Attribute::Players);

	object3d_.reset(KObject3d::Create(
	    ModelManager::GetInstance()->GetModels("Explotion"),
	    PipelineManager::GetInstance()->GetObjPipeline()));
	object3d_->GetTransform().SetScale({0.0f, 0.0f, 0.0f});

	audioManager_ = AudioManager::GetInstance();
}

void Explosion::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	if (!isDead_) {
		if (expTimer_ <= expTime_) {
			if (expTimer_ == 0) {
				audioManager_->SEPlay_wav("bakuhatuSE.wav", 0.75f);
			}

			expTimer_++;

			KMyMath::Vector3 start = {0.0f, 0.0f, 0.0f};
			KMyMath::Vector3 end = {75.0f, 75.0f, 75.0f};

			KMyMath::Vector3 scale = MyEase::OutCubicVec3(start, end, expTimer_ / expTime_);
			object3d_->GetTransform().SetScale(scale);

			expAlpha_ = MyEase::InQuadFloat(255.0f, 0.0f, expTimer_ / expTime_);
			object3d_->SetRGB(expRGB_);
			object3d_->SetAlpha(expAlpha_);
		} else {
			isDead_ = true;
		}
	}

	object3d_->Update(viewPro, cameraPos);
}

void Explosion::Draw() { object3d_->Draw(); }

void Explosion::Set(const KMyMath::Vector3& pos, const float time) { 
	isDead_ = false; 
	object3d_->GetTransform().SetPos(pos);
	expTime_ = time;
}

void Explosion::SetIsDead(bool isDead) { isDead_ = isDead; }

const bool& Explosion::GetIsDead() const { return isDead_; }

void Explosion::OnCollision() {}

KMyMath::Vector3 Explosion::GetWorldPosition() {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = object3d_->GetTransform().GetWorldPos();

	return result;
}
