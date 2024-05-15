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

#include "BulletManager.h"

Blaster* Blaster::nowBlaster = nullptr;

std::unique_ptr<ActState> Blaster::blasterActState = nullptr;

Blaster* Blaster::Create(
    KGPlin* pipeline_, const KMyMath::Vector3& pos_, KGPlin* spritePipeline_) {
	// インスタンス生成
	Blaster* blaster = new Blaster();
	if (blaster == nullptr) {
		return nullptr;
	}

	// 初期化
	blaster->Init(pipeline_, pos_, spritePipeline_);

	return blaster;
}

void Blaster::Init(
    KGPlin* pipeline_, const KMyMath::Vector3& initPos_, KGPlin* spritePipeline_) {
	model = ModelManager::GetInstance()->GetModels("Blaster_Core");

	BossEnemy::Init(pipeline_, initPos_, spritePipeline_);

	maxHP = 300;
	HP = maxHP;

	for (size_t i = 0; i < 8; i++) {
		units[i].reset(KObject3d::Create(
		    ModelManager::GetInstance()->GetModels("Blaster_Unit"), pipeline_));

		units[i]->SetParent(&object3d->GetTransform());
	}

	// スタート時のユニットの位置
	units[0]->GetTransform().SetPos({3.0f, 3.0f, 3.0f});
	units[1]->GetTransform().SetPos({-3.0f, 3.0f, 3.0f});
	units[2]->GetTransform().SetPos({3.0f, 3.0f, -3.0f});
	units[3]->GetTransform().SetPos({-3.0f, 3.0f, -3.0f});
	units[4]->GetTransform().SetPos({3.0f, -3.0f, 3.0f});
	units[5]->GetTransform().SetPos({-3.0f, -3.0f, 3.0f});
	units[6]->GetTransform().SetPos({3.0f, -3.0f, -3.0f});
	units[7]->GetTransform().SetPos({-3.0f, -3.0f, -3.0f});

	// レティクル
	reticle2d = std::make_unique<Reticle2D>();
	reticle2d->Init();
	reticle2d->SetColor({1, 0, 0});

	blasterActState = std::make_unique<BlasterStandState>();
}

void Blaster::Update(
    ViewProjection* viewPro_, const KMyMath::Vector3& cameraPos, bool isBossMovie_) {
	isBossMovie = isBossMovie_;
	viewPro = viewPro_;

	if (!isDead) {
		if (isBossMovie) {

		} else {
			if (blasterActState->GetIsFinish()) {
				actSelect = (size_t)MyMathUtility::GetRandI(1, 3);
				/*switch (actSelect) {
				case 1:
					blasterActState = std::make_unique<BlasterAimState>();
					break;
				case 2:
					blasterActState = std::make_unique<BlasterTackleState>();
					isReticle = true;
					break;
				case 3:
					blasterActState = std::make_unique<BlasterUnitLazer>();
					break;
				default:
					blasterActState = std::make_unique<BlasterStandState>();
					break;
				}*/
				blasterActState = std::make_unique<BlasterTackleState>();
				isReticle = true;
			}

			if (blasterActState) {
				blasterActState->Update();
			}
		}

		if (HP <= min(HP, 0)) {
			BulletManager::GetInstance()->AllBulletDelete();
			ScoreManager::GetInstance()->AddBossScore(100000);
			isDead = true;
		}
	} else {
		DeadEffect();
	}

	// HP演出
	HPEffect();

	object3d->Update(viewPro_, cameraPos);

	for (size_t i = 0; i < 8; i++) {
		units[i]->Update(viewPro_, cameraPos);
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

void Blaster::UIDraw() {
	if (isBossMovie || isDead) {
		return;
	}

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());

	const KMyMath::Vector2 hpTexSize = {672.0f, 22.0f};

	const float sizeX = hpTexSize.x / maxHP;
	const float sizeY = hpTexSize.y;

	HPBarUI->Draw(hpbarTex, {width / 2.0f, HPPos.y}, {1.0f, 1.0f}, 0, {1.0f, 1.0f, 1.0f, 1.0f});

	const KMyMath::Vector2 HPUIPos = {(width / 2) - (hpTexSize.x / 2), HPPos.y};

	HPrectUI->Draw(
	    hpTex, HPUIPos, {oldHP * sizeX, sizeY}, 0, {1, 0, 0, 0.3f}, false, false, {0, 0.5f});

	HPUI->Draw(hpTex, HPUIPos, {HP * sizeX, sizeY}, 0, {1, 0, 0, 1}, false, false, {0, 0.5f});

	if (isReticle) {
		reticle2d->Draw();
	}
}

bool Blaster::CollisionCheck(const KMyMath::Vector3& posA_, const KMyMath::Vector3& posB_) {
	if (MyCollisions::CheckSphereToSphere(posA_, posB_, 2, 12)) {
		return true;
	}

	return false;
}

void Blaster::SetFarstAct() { blasterActState = std::make_unique<BlasterStandState>(); }

const KMyMath::Vector3 Blaster::GetUnitsPos(size_t num_) const {
	return units[num_]->GetTransform().GetPos();
}

const KMyMath::Vector3 Blaster::GetUnitsScale(size_t num_) const {
	return units[num_]->GetTransform().GetScale();
}

void Blaster::SetUnitsPos(const KMyMath::Vector3& pos_, size_t num_) {
	units[num_]->GetTransform().SetPos(pos_);
}

void Blaster::SetUnitsScale(const KMyMath::Vector3& scale_, size_t num_) {
	units[num_]->GetTransform().SetScale(scale_);
}

void Blaster::AddSetPos(const KMyMath::Vector3& pos_) { object3d->GetTransform().AddSetPos(pos_); }

const KMyMath::Vector3 Blaster::UnitsGetWorldPos(uint32_t& i) const {
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result = units[i]->GetTransform().GetWorldPos();

	return result;
}
