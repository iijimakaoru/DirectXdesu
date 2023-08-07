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
	this->HP = 0;

	HPUI = std::make_unique<Sprite>();
	HPUI->Init();
	HPUI->SetPipeline(spritePipeline);

	hpTex = TextureManager::Load("Resources/texture/white1x1.png");

	HPBarUI = std::make_unique<Sprite>();
	HPBarUI->Init();
	HPBarUI->SetPipeline(spritePipeline);

	hpbarTex = TextureManager::Load("Resources/texture/PlayersHPBar.png");

	isHPE = true;
}

void BossEnemy::HPGauge()
{
	easeTimer += 0.05f;
	HP = MyEase::OutQuadFloat(0, maxHP, easeTimer);

	if (HP >= max(HP, maxHP))
	{
		isHPE = false;
	}
}

void BossEnemy::Draw()
{
	object3d->Draw();
}

void BossEnemy::UIDraw()
{
	HPUI->Draw(hpTex, { 901,11 }, { HP * (318 / maxHP),30 }, 0, { 1,0,0,1 }, false, false, { 0,0 });

	HPBarUI->Draw(hpbarTex, { 900,10 }, { 1,1 }, 0, { 1,1,1,1 }, false, false, { 0,0 });
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
}

const bool BossEnemy::GetIsHPE() const
{
	return isHPE;
}
