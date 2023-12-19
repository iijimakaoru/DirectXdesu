/**
 * @file BossEnemy.cpp
 * @brief ボスの基底
 * @author 飯島 薫
 */

#include "BossEnemy.h"
#include "ParticleManager.h"
#include "Ease.h"
#include "Collision.h"

void BossEnemy::Init(KGPlin* pipeline_, const KMyMath::Vector3& initPos_, KGPlin* spritePipeline_)
{
	// パイプライン生成
	pipeline = pipeline_;
	spritePipeline = spritePipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model.get(), pipeline));
	object3d->SetPos(initPos_);

	// 体力セット
	maxHP = 0;
	HP = maxHP;

	HPUI = std::make_unique<Sprite>();
	HPUI->Init();
	HPUI->SetPipeline(spritePipeline);

	HPrectUI = std::make_unique<Sprite>();
	HPrectUI->Init();
	HPrectUI->SetPipeline(spritePipeline);

	hpTex = TextureManager::Load("Resources/texture/white1x1.png");

	HPBarUI = std::make_unique<Sprite>();
	HPBarUI->Init();
	HPBarUI->SetPipeline(spritePipeline);

	hpbarTex = TextureManager::Load("Resources/texture/BossHPBar3.png");

	isHPE = true;

	isDead = false;

	HPPos = {204.0f, 40.0f};
}

void BossEnemy::Draw()
{
	object3d->Draw();
}

void BossEnemy::UIDraw()
{
	if (isBossMovie || isDead)
	{
		return;
	}

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());

	const KMyMath::Vector2 hpTexSize = {672.0f, 22.0f};

	const float sizeX = hpTexSize.x / maxHP;
	const float sizeY = hpTexSize.y;

	HPBarUI->Draw(
	    hpbarTex, {width / 2.0f, HPPos.y}, {1.0f, 1.0f}, 0, {1.0f, 1.0f, 1.0f, 1.0f});

	const KMyMath::Vector2 HPUIPos = {(width / 2) - (hpTexSize.x / 2), HPPos.y};

	HPrectUI->Draw(hpTex, HPUIPos, { oldHP * sizeX,sizeY }, 0, { 1,0,0,0.3f }, false, false, { 0,0.5f });

	HPUI->Draw(hpTex, HPUIPos, { HP * sizeX,sizeY }, 0, { 1,0,0,1 }, false, false, { 0,0.5f });
}

const KMyMath::Vector3 BossEnemy::GetWorldPos() const
{
	// ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->GetMatWorld().m[3][0];
	result.y = object3d->GetMatWorld().m[3][1];
	result.z = object3d->GetMatWorld().m[3][2];

	return result;
}

void BossEnemy::OnCollision()
{
	ObjParticleManager::GetInstance()->SetExp(GetWorldPos());
	HP--;
	hpEase = true;
	oldHpTimer = 0;
	hpEaseTimer = 0;
}

bool BossEnemy::CollisionCheck(const KMyMath::Vector3& posA_, const KMyMath::Vector3& posB_)
{
	if (MyCollisions::CheckSphereToSphere(posA_, posB_, 1, 1))
	{
		return true;
	}

	return false;
}

void BossEnemy::DeadEffect()
{
	if (!isFallEffectEnd)
	{
		// 姿勢制御
		object3d->SetRot({ 0,0,object3d->GetRot().z });

		// 回転
		object3d->SetRot(MyEase::Lerp3D({ 0.0f,0.0f,0.0f }, { 20.0f,0.0f,20.0f }, fallEffectTimer / fallEffectTime));

		// 落下
		object3d->AddSetPos({ 0.0f,-0.15f,0.0f });

		// 時間経過
		fallEffectTimer++;

		// 
		expTimer++;

		if (expTimer >= max(expTimer, expTime))
		{
			ObjParticleManager::GetInstance()->SetSmallExp({ GetWorldPos().x + MyMathUtility::GetRand(-10.0f,10.0f),
			GetWorldPos().y + MyMathUtility::GetRand(-10.0f,10.0f),GetWorldPos().z + MyMathUtility::GetRand(-10.0f,10.0f) });
			expTimer = 0;
		}

		// 演出終わり
		if (fallEffectTimer >= max(fallEffectTimer, fallEffectTime))
		{
			ObjParticleManager::GetInstance()->SetExp(GetWorldPos());
			isFallEffectEnd = true;
		}
	}
}

void BossEnemy::HPEffect()
{
	if (hpEase)
	{
		if (oldHpTimer < oldHpTime)
		{
			oldHpTimer++;
		}
		else
		{
			hpEaseTimer++;

			oldHP = MyEase::OutCubicFloat(startHpEase, HP, hpEaseTimer / hpEaseTime);

			if (hpEaseTimer > hpEaseTime)
			{
				hpEase = false;
			}
		}
	}
	else
	{
		oldHP = HP;
		startHpEase = oldHP;
		oldHpTimer = 0;
		hpEaseTimer = 0;
	}
}

const bool BossEnemy::GetIsHPE() const
{
	return isHPE;
}

const bool BossEnemy::GetIsDead() const
{
	return isDead;
}

const bool BossEnemy::GetIsFallEffectEnd() const
{
	return isFallEffectEnd;
}

const KMyMath::Vector3& BossEnemy::GetRot() const
{
	return object3d->GetRot();
}

void BossEnemy::SetPos(const KMyMath::Vector3& pos_)
{
	object3d->SetPos(pos_);
}

void BossEnemy::SetRot(const KMyMath::Vector3& rot_)
{
	object3d->SetRot(rot_);
}
