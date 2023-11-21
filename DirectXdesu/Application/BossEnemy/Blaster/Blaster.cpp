/**
 * @file Blaster.cpp
 * @brief ボス1 : ブラスター
 * @author 飯島 薫
 */

#include "Blaster.h"
#include "Collision.h"
#include "BlasterStandState.h"

Blaster* Blaster::nowBlaster = nullptr;

Blaster* Blaster::Create(KGPlin* pipeline_, const KMyMath::Vector3& pos_,
	const float HP, KGPlin* spritePipeline_)
{
	// インスタンス生成
	Blaster* blaster = new Blaster();
	if (blaster == nullptr)
	{
		return nullptr;
	}

	// 初期化
	blaster->Init(pipeline_, pos_, HP, spritePipeline_);

	return blaster;
}

void Blaster::Init(KGPlin* pipeline_, const KMyMath::Vector3& initPos_,
	const float HP_, KGPlin* spritePipeline_)
{
	model = std::make_unique<MtlObj>("BlasterCore");
	model->CreateModel();

	BossEnemy::Init(pipeline_, initPos_, HP_, spritePipeline_);

	unitsModel = std::make_unique<MtlObj>("BlasterUnit");
	unitsModel->CreateModel();

	for (size_t i = 0; i < 8; i++)
	{
		units[i].reset(KObject3d::Create(unitsModel.get(), pipeline_));

		units[i]->SetParent(&object3d->GetTransform());
	}

	// スタート時のユニットの位置
	units[0]->SetPos({  3.0f, 3.0f, 3.0f });
	units[1]->SetPos({ -3.0f, 3.0f, 3.0f });
	units[2]->SetPos({ -3.0f,-3.0f, 3.0f });
	units[3]->SetPos({  3.0f,-3.0f, 3.0f });
	units[4]->SetPos({  3.0f, 3.0f,-3.0f });
	units[5]->SetPos({ -3.0f, 3.0f,-3.0f });
	units[6]->SetPos({ -3.0f,-3.0f,-3.0f });
	units[7]->SetPos({  3.0f,-3.0f,-3.0f });

	// 
	actState = std::make_unique<BlasterStandState>();
}

void Blaster::Update(ViewProjection* viewPro_, bool isBossMovie_)
{
	if (isBossMovie_)
	{

	}
	else
	{
		if (actState->GetIsFinish())
		{
			actState = std::make_unique<BlasterStandState>();
		}

		if (actState)
		{
			actState->Update();
		}
	}

	BossEnemy::Update(viewPro_, isBossMovie_);

	for (size_t i = 0; i < 8; i++)
	{
		units[i]->Update(viewPro_);
	}
}

void Blaster::Draw()
{
	if (isFallEffectEnd)
	{
		return;
	}

	BossEnemy::Draw();

	for (size_t i = 0; i < 8; i++)
	{
		units[i]->Draw();
	}
}

bool Blaster::CollisionCheck(const KMyMath::Vector3& posA_, const KMyMath::Vector3& posB_)
{
	if (MyCollisions::CheckSphereToSphere(posA_, posB_, 2, 12))
	{
		return true;
	}

	return false;
}

void Blaster::SetUnitsPos(const KMyMath::Vector3& pos_, size_t num_)
{
	units[num_]->SetPos(pos_);
}
