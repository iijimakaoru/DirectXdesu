#include "MobEnemy.h"
#include "MyMath.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#include "Player.h"

MobEnemy::MobEnemy()
{
}

MobEnemy::~MobEnemy()
{
}

void MobEnemy::Init(KModel* model_, KGPlin* pipeline_)
{
	// モデル生成
	model = model_;

	// パイプライン
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));

	isDead = false;
}

void MobEnemy::Update(ViewProjection* viewPro_, const KMyMath::Vector3& cameraPos_)
{
	cameraPos = cameraPos_;

	object3d->Update(viewPro_);
}

void MobEnemy::Draw()
{
	if (!isDead)
	{
		object3d->Draw();
	}
}

void MobEnemy::OnCollision()
{
	ObjParticleManager::GetInstance()->SetSmallExp(GetWorldPos());
	isDead = true;
}

const bool MobEnemy::GetIsDelete() const { return isDelete; }

void MobEnemy::SetCameraPos(const KMyMath::Vector3& cameraPos_) {
	cameraPos = cameraPos_;
}

void MobEnemy::SelfDestruction()
{
	isDelete = true;
}

void MobEnemy::Attack() {}

const KMyMath::Vector3 MobEnemy::GetWorldPos() const {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->GetMatWorld().m[3][0];
	result.y = object3d->GetMatWorld().m[3][1];
	result.z = object3d->GetMatWorld().m[3][2];

	return result;
}