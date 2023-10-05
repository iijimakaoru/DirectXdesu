#include "Blaster.h"
#include "Collision.h"

Blaster* Blaster::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos,
	const float HP, KGPlin* spritePipeline_)
{
	// インスタンス生成
	Blaster* blaster = new Blaster();
	if (blaster == nullptr)
	{
		return nullptr;
	}

	// 初期化
	blaster->Init(model_, pipeline_, pos, HP, spritePipeline_);

	return blaster;
}

void Blaster::Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos,
	const float HP, KGPlin* spritePipeline_)
{
	input = KInput::GetInstance();

	BossEnemy::Init(model_, pipeline_, initPos, HP, spritePipeline_);

	object3d->transform.scale = { 10.0f,10.0f,10.0f };
}

void Blaster::Update(ViewProjection* viewPro)
{
	BossEnemy::Update(viewPro);
}

bool Blaster::CollisionCheck(const KMyMath::Vector3& posA, const KMyMath::Vector3& posB)
{
	if (MyCollisions::CheckSphereToSphere(posA, posB, 12, 2))
	{
		return true;
	}

	return false;
}