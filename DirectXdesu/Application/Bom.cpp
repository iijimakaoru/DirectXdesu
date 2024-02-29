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
	object3d->SetScale({10.0f, 10.0f, 10.0f});
	expObject.reset(KObject3d::Create(expModel, pipeline));
	expObject->SetScale({0.0f, 0.0f, 0.0f});
}

void Bom::Update(ViewProjection* viewPro_) {
	if (!isExp) {
		lifeTimer--;
		object3d->AddSetPos(vec);

		expObject->SetPos(object3d->GetPos());

		if (lifeTimer <= 0) {
			isExp = true;
		}
	} else {
		if (expTimer <= expTime) {
			expTimer++;

			KMyMath::Vector3 start = {0.0f, 0.0f, 0.0f};
			KMyMath::Vector3 end = {75.0f, 75.0f, 75.0f};

			KMyMath::Vector3 scale = MyEase::OutCubicVec3(start, end, expTimer / expTime);
			expObject->SetScale(scale);

			expAlpha = MyEase::InQuadFloat(255.0f, 0.0f, expTimer / expTime);
			expObject->SetRGB(expRGB);
			expObject->SetAlpha(expAlpha);
		} else {
			isDead = true;
		}
	}

	object3d->Update(viewPro_);
	expObject->Update(viewPro_);
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
	object3d->SetPos(pos_);
	this->vec = vec_;
	vec = MyMathUtility::MakeNormalize(vec) * bulletSpeed_;
	object3d->SetRot(rot_);

	lifeTimer = 60;

	isExp = false;

	isDead = false;
}

void Bom::OnCollision() { isExp = true; }

void Bom::SetIsDead(bool isDead_) { isDead = isDead_; }

const bool& Bom::GetIsDead() const { return isDead; }

const bool& Bom::GetIsExp() const { return isExp; }

KMyMath::Vector3 Bom::GetWorldPos() {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->GetMatWorld().m[3][0];
	result.y = object3d->GetMatWorld().m[3][1];
	result.z = object3d->GetMatWorld().m[3][2];

	return result;
}

const float& Bom::GetBomsPower() const { return bomPower; }

const float& Bom::GetExpPower() const { return expPower; }

void Bom::SetBomHit(bool isBomHit_) { isBomHit = isBomHit_; }

const bool& Bom::GetBomHit() const { return isBomHit; }
