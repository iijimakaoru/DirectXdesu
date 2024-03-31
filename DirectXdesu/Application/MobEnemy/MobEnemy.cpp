#include "MobEnemy.h"
#include "BulletManager.h"
#include "MyMath.h"
#include "ParticleManager.h"
#include "Player.h"

MobEnemy::MobEnemy() {}

MobEnemy::~MobEnemy() {}

void MobEnemy::Init(KModel* model_, KGPlin* pipeline_) {
	SetCollisionAttribute(Collider::Attribute::Enemys);
	SetCollisionMask((uint32_t)~Collider::Attribute::EnemysBullet);

	// モデル生成
	model = model_;

	// パイプライン
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));

	isDead = false;
}

void MobEnemy::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	object3d->Update(viewPro, cameraPos);
}

void MobEnemy::Draw() {
	if (!isDead) {
		object3d->Draw();
	}
}

void MobEnemy::OnCollision() {
	ObjParticleManager::GetInstance()->SetSmallExp(GetWorldPos());
	isDead = true;
}

KMyMath::Vector3 MobEnemy::GetWorldPosition() { 
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = object3d->GetTransform().GetWorldPos();

	return result;
}

const bool MobEnemy::GetIsDelete() const { return isDelete; }

void MobEnemy::SelfDestruction() { isDelete = true; }

void MobEnemy::Attack() {}

const KMyMath::Vector3 MobEnemy::GetWorldPos() const {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = object3d->GetTransform().GetWorldPos();

	return result;
}