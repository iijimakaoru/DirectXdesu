/**
 * @file Blaster.cpp
 * @brief ボス1 : ブラスター
 * @author 飯島 薫
 */

#include "Blaster.h"
#include "Collision.h"

Blaster* Blaster::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_,
	const float HP, KGPlin* spritePipeline_)
{
	// インスタンス生成
	Blaster* blaster = new Blaster();
	if (blaster == nullptr)
	{
		return nullptr;
	}

	// 初期化
	blaster->Init(model_, pipeline_, pos_, HP, spritePipeline_);

	return blaster;
}

void Blaster::Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos_,
	const float HP_, KGPlin* spritePipeline_)
{
	BossEnemy::Init(model_, pipeline_, initPos_, HP_, spritePipeline_);

	object3d->SetScale({ 10.0f,10.0f,10.0f });
}

void Blaster::Update(ViewProjection* viewPro)
{
	BossEnemy::Update(viewPro);
}

bool Blaster::CollisionCheck(const KMyMath::Vector3& posA_, const KMyMath::Vector3& posB_)
{
	if (MyCollisions::CheckSphereToSphere(posA_, posB_, 2, 12))
	{
		return true;
	}

	return false;
}
