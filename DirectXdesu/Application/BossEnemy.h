#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Sprite.h"

class BossEnemy
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="pipeline_"></param>
	/// <param name="initPos"></param>
	/// <param name="HP"></param>
	virtual void Init(KModel* model_, KGPlin* pipeline_,const KMyMath::Vector3& initPos,
		const float HP, KGPlin* spritePipeline_);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro);

	/// <summary>
	/// 
	/// </summary>
	void HPGauge(const uint32_t& easeTime);

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();
	virtual void UIDraw();

	/// <summary>
	/// ���[���h���W�Q�b�^�[
	/// </summary>
	/// <returns></returns>
	virtual const KMyMath::Vector3 GetWorldPos() const;

	// �����������̏���
	virtual void OnCollision();

	// �����蔻��͈�
	virtual bool CollisionCheck(const KMyMath::Vector3& posA, const KMyMath::Vector3& posB) = 0;

	// ���S���o
	virtual void DeadEffect();

	// HP���o
	void HPEffect();

	const bool GetIsHPE() const;

	const bool GetIsDead() const;

	const bool GetIsFallEffectEnd() const;

protected:
	// �g�����X�t�H�[��
	std::unique_ptr<KObject3d> object3d = nullptr;

	// ���f��
	KModel* model = nullptr;

	// �p�C�v���C��
	KGPlin* pipeline = nullptr;
	KGPlin* spritePipeline = nullptr;

	// ����ł邩
	bool isDead = false;

#pragma region HP
	// �ő�̗�
	float maxHP;

	// ���̗�
	float HP;
	float oldHP;
	float startHpEase;

	uint32_t oldHpTimer = 0;
	const uint32_t oldHpTime = 60;

	bool hpEase = false;
	float hpEaseTimer = 0;
	const float hpEaseTime = 15;

	// HP
	std::unique_ptr<Sprite> HPUI = nullptr;
	TextureData hpTex;
	std::unique_ptr<Sprite> HPrectUI = nullptr;

	// HP�o�[
	std::unique_ptr<Sprite> HPBarUI = nullptr;
	TextureData hpbarTex;

	uint32_t easeTimer = 0;
	bool isHPE = false;
#pragma endregion

#pragma region ���j���o�p
	// ���o����
	float fallEffectTime = 90;
	float fallEffectTimer = 0;

	// �����N�[���^�C��
	float expTime = 5;
	float expTimer = 0;

	bool isFallEffectEnd = false;
#pragma endregion

};

