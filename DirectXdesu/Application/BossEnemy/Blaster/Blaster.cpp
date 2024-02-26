/**
 * @file Blaster.cpp
 * @brief ボス1 : ブラスター
 * @author 飯島 薫
 */

#include "Blaster.h"
#include "BlasterAimState.h"
#include "BlasterStandState.h"
#include "BlasterTackleState.h"
#include "BlasterUnitLazer.h"
#include "Collision.h"
#include "ImguiManager.h"
#include "ModelManager.h"
#include "ScoreManager.h"

Blaster* Blaster::nowBlaster = nullptr;

std::unique_ptr<ActState> Blaster::blasterActState = nullptr;

Blaster* Blaster::Create(KGPlin* pipeline_, const KMyMath::Vector3& pos_, KGPlin* spritePipeline_) {
	// インスタンス生成
	Blaster* blaster = new Blaster();
	if (blaster == nullptr) {
		return nullptr;
	}

	// 初期化
	blaster->Init(pipeline_, pos_, spritePipeline_);

	return blaster;
}

void Blaster::Init(KGPlin* pipeline_, const KMyMath::Vector3& initPos_, KGPlin* spritePipeline_) {
	model = ModelManager::GetInstance()->GetModels("Blaster_Core");

	BossEnemy::Init(pipeline_, initPos_, spritePipeline_);

	maxHP = 300;
	HP = maxHP;

	for (size_t i = 0; i < 8; i++) {
		units[i].reset(
		    KObject3d::Create(ModelManager::GetInstance()->GetModels("Blaster_Unit"), pipeline_));

		units[i]->SetParent(&object3d->GetTransform());
	}

	// スタート時のユニットの位置
	units[0]->SetPos({3.0f, 3.0f, 3.0f});
	units[1]->SetPos({-3.0f, 3.0f, 3.0f});
	units[2]->SetPos({3.0f, 3.0f, -3.0f});
	units[3]->SetPos({-3.0f, 3.0f, -3.0f});
	units[4]->SetPos({3.0f, -3.0f, 3.0f});
	units[5]->SetPos({-3.0f, -3.0f, 3.0f});
	units[6]->SetPos({3.0f, -3.0f, -3.0f});
	units[7]->SetPos({-3.0f, -3.0f, -3.0f});

	blasterActState = std::make_unique<BlasterStandState>();
}

void Blaster::Update(ViewProjection* viewPro_, bool isBossMovie_) {
	isBossMovie = isBossMovie_;

	if (!isDead) {
		if (isBossMovie) {

		} else {
			if (blasterActState->GetIsFinish()) {
				actSelect = (size_t)MyMathUtility::GetRandI(1, 2);
				switch (actSelect) {
				case 1:
				    blasterActState = std::make_unique<BlasterAimState>();
				    break;
				case 2:
				    blasterActState = std::make_unique<BlasterTackleState>();
				    break;
				case 3:
				    blasterActState = std::make_unique<BlasterUnitLazer>();
				    break;
				default:
				    blasterActState = std::make_unique<BlasterStandState>();
				    break;
				}
			}

			if (blasterActState) {
				blasterActState->Update();
			}
		}

		if (HP <= min(HP, 0)) {
			ScoreManager::GetInstance()->AddBossScore(100000);
			isDead = true;
		}
	} else {
		DeadEffect();
	}

	// HP演出
	HPEffect();

	object3d->Update(viewPro_);

	for (size_t i = 0; i < 8; i++) {
		units[i]->Update(viewPro_);
	}
}

void Blaster::Draw() {
	if (isFallEffectEnd) {
		return;
	}

	BossEnemy::Draw();

	for (size_t i = 0; i < 8; i++) {
		units[i]->Draw();
	}
}

bool Blaster::CollisionCheck(const KMyMath::Vector3& posA_, const KMyMath::Vector3& posB_) {
	if (MyCollisions::CheckSphereToSphere(posA_, posB_, 2, 12)) {
		return true;
	}

	return false;
}

void Blaster::SetFarstAct() { blasterActState = std::make_unique<BlasterStandState>(); }

const KMyMath::Vector3 Blaster::GetUnitsPos(size_t num_) const { return units[num_]->GetPos(); }

const KMyMath::Vector3 Blaster::GetUnitsScale(size_t num_) const { return units[num_]->GetScale(); }

void Blaster::SetUnitsPos(const KMyMath::Vector3& pos_, size_t num_) { units[num_]->SetPos(pos_); }

void Blaster::SetUnitsScale(const KMyMath::Vector3& scale_, size_t num_) {
	units[num_]->SetScale(scale_);
}

void Blaster::AddSetPos(const KMyMath::Vector3& pos_) { object3d->AddSetPos(pos_); }
