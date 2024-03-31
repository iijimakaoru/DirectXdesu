#include "Bom.h"
#include "BulletManager.h"
#include "Ease.h"

Bom* Bom::Create(
    KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_,
    const KMyMath::Vector3& rot_, const float bulletSpeed_) {
	// インスタンス生成
	Bom* bom = new Bom();
	if (bom == nullptr) {
		return nullptr;
	}

	// 初期化
	bom->Init(model_, pipeline_);
	bom->Set(pos_, vec_, rot_, bulletSpeed_);

	return bom;
}

void Bom::Init(KModel* model_, KGPlin* pipeline_) {
	SetCollisionAttribute(Collider::Attribute::Players);
	SetCollisionMask((uint32_t)~Collider::Attribute::Players);

	// モデル生成
	model = model_;

	// パイプライン生成
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->GetTransform().SetScale({10.0f, 10.0f, 10.0f});

	audioManager = AudioManager::GetInstance();
}

void Bom::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	lifeTimer--;
	object3d->GetTransform().AddSetPos(vec);

	if (lifeTimer <= 0 || isExp) {
		BulletManager::GetInstance()->ExpCall(object3d->GetTransform().GetWorldPos(), 30.0f);
		isDead = true;
	}

	object3d->Update(viewPro, cameraPos);
}

void Bom::Draw() { object3d->Draw(); }

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

KMyMath::Vector3 Bom::GetWorldPosition() {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = object3d->GetTransform().GetWorldPos();

	return result;
}

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
