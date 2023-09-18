#include "BossEnemy.h"
#include "ParticleManager.h"
#include "Ease.h"

void BossEnemy::Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos, const float HP, KGPlin* spritePipeline_)
{
	// ���f������
	model = model_;

	// �p�C�v���C������
	pipeline = pipeline_;
	spritePipeline = spritePipeline_;

	// �I�u�W�F�N�g����
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->transform.pos = initPos;

	// �̗̓Z�b�g
	maxHP = HP;
	this->HP = 1;

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

	hpbarTex = TextureManager::Load("Resources/texture/PlayersHPBar.png");

	isHPE = true;

	isDead = false;
}

void BossEnemy::Update(ViewProjection* viewPro)
{
	if (!isDead)
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

		if (HP <= min(HP, 0))
		{
			ParticleManager::GetInstance()->CallExp(GetWorldPos());
			isDead = true;
		}
	}
	else
	{
		DeadEffect();
	}

	object3d->Update(viewPro);
}

void BossEnemy::HPGauge(const uint32_t& easeTime)
{
	easeTimer ++;
	HP = MyEase::OutQuadFloat(1, maxHP, (float)easeTimer / easeTime);

	if (HP >= max(HP, maxHP))
	{
		isHPE = false;
	}
}

void BossEnemy::Draw()
{
	if (!isFallEffectEnd)
	{
		object3d->Draw();
	}
}

void BossEnemy::UIDraw()
{
	HPBarUI->Draw(hpbarTex, { 900,10 }, { 1,1 }, 0, { 1,1,1,1 }, false, false, { 0,0 });

	HPrectUI->Draw(hpTex, { 901,11 }, { oldHP * (318 / maxHP),30 }, 0, { 1,0,0,0.3f }, false, false, { 0,0 });

	HPUI->Draw(hpTex, { 901,11 }, { HP * (318 / maxHP),30 }, 0, { 1,0,0,1 }, false, false, { 0,0 });
}

const KMyMath::Vector3 BossEnemy::GetWorldPos() const
{
	// ���[���h���W�i�[�ϐ�
	KMyMath::Vector3 result;

	// ���[���h�s��̕��s�ړ������擾
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}

void BossEnemy::OnCollision()
{
	ParticleManager::GetInstance()->CallExp(GetWorldPos());
	HP--;
	hpEase = true;
	oldHpTimer = 0;
	hpEaseTimer = 0;
}

void BossEnemy::DeadEffect()
{
	if (!isFallEffectEnd)
	{
		// �p������
		object3d->transform.rot.x = 0;
		object3d->transform.rot.y = 0;

		// ��]
		object3d->transform.rot.z += 10.0f;

		// ����
		object3d->transform.pos.y -= 0.1f;

		// ���Ԍo��
		fallEffectTimer++;

		// 
		expTimer++;

		if (expTimer >= max(expTimer, expTime))
		{
			ParticleManager::GetInstance()->CallSmallExp({ GetWorldPos().x + MyMathUtility::GetRand(-3.0f,3.0f),
			GetWorldPos().y + MyMathUtility::GetRand(-3.0f,3.0f),GetWorldPos().z + MyMathUtility::GetRand(-3.0f,3.0f) });
			expTimer = 0;
		}

		// ���o�I���
		if (fallEffectTimer >= max(fallEffectTimer, fallEffectTime))
		{
			ParticleManager::GetInstance()->CallExp(GetWorldPos());
			isFallEffectEnd = true;
		}
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
