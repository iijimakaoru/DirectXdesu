#include "Bom.h"
#include "Ease.h"

Bom* Bom::Create(
    KModel* model_, KModel* expModel_, KGPlin* pipeline_, const KMyMath::Vector3& pos_,
    const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed_) {
	// インスタンス生成
	Bom* bom = new Bom();
	if (bom == nullptr) {
		return nullptr;
	}

	// 初期化
	bom->Init(model_, expModel_, pipeline_);
	bom->Set(pos_, vec_, rot_, bulletSpeed_);

	return bom;
}

void Bom::Init(KModel* model_, KModel* expModel_, KGPlin* pipeline_) {
	// モデル生成
	model = model_;
	expModel = expModel_;

	// パイプライン生成
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->GetTransform().SetScale({10.0f, 10.0f, 10.0f});
	expObject.reset(KObject3d::Create(expModel, pipeline));
	expObject->GetTransform().SetScale({0.0f, 0.0f, 0.0f});

	audioManager = AudioManager::GetInstance();
}

void Bom::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	if (!isExp) {
		lifeTimer--;
		object3d->GetTransform().AddSetPos(vec);

		expObject->GetTransform().SetPos(object3d->GetTransform().GetPos());

		if (lifeTimer <= 0) {
			isExp = true;
		}
	} else {
		if (expTimer <= expTime) {
			if (expTimer == 0) {
				audioManager->SEPlay_wav("bakuhatuSE.wav", 0.75f);
			}

			expTimer++;

			KMyMath::Vector3 start = {0.0f, 0.0f, 0.0f};
			KMyMath::Vector3 end = {75.0f, 75.0f, 75.0f};

			KMyMath::Vector3 scale = MyEase::OutCubicVec3(start, end, expTimer / expTime);
			expObject->GetTransform().SetScale(scale);

			expAlpha = MyEase::InQuadFloat(255.0f, 0.0f, expTimer / expTime);
			expObject->SetRGB(expRGB);
			expObject->SetAlpha(expAlpha);
		} else {
			isDead = true;
		}
	}

	object3d->Update(viewPro, cameraPos);
	expObject->Update(viewPro, cameraPos);
}

void Bom::Draw() {
	if (!isExp) {
		object3d->Draw();
	} else {
		expObject->Draw();
	}
}

void Bom::Set(
    const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
    const float bulletSpeed_) {
	object3d->GetTransform().SetPos(pos_);
	this->vec = vec_;
	vec = MyMathUtility::MakeNormalize(vec) * bulletSpeed_;
	object3d->GetTransform().SetRot(rot_);

	lifeTimer = 40;

	isExp = false;

	isDead = false;
}

void Bom::OnCollision() { isExp = true; }

void Bom::SetIsDead(bool isDead_) { isDead = isDead_; }

const bool& Bom::GetIsDead() const { return isDead; }

const bool& Bom::GetIsExp() const { return isExp; }

const float& Bom::GetBomsPower() const { return bomPower; }

const float& Bom::GetExpPower() const { return expPower; }

void Bom::SetBomHit(bool isBomHit_) { isBomHit = isBomHit_; }

const bool& Bom::GetBomHit() const { return isBomHit; }

const KMyMath::Vector3 Bom::GetWorldPos() const {
	KMyMath::Vector3 result;

	result = object3d->GetTransform().GetWorldPos();

	return result;
}
