#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"
#include "KInput.h"
#include "PlayerBullet.h"

#include "Reticle3D.h"
#include "Reticle2D.h"

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="model_"></param>
	/// <returns></returns>
	static Player* Create(KModel* model_,KGPlin* objPipeline_, const float playerHP, KGPlin* spritePipeline_);

public:
	// ������
	void Init(KModel* model_, KGPlin* objPipeline_, const float playerHP, KGPlin* spritePipeline_);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera_"></param>
	void Update(ViewProjection* viewPro);

	// �`��
	// �I�u�W�F�N�g
	void ObjDraw();

	// �Q�[���X�v���C�g
	void SpriteDraw();

	// UI
	void UIDraw();

	// �Փˎ��ɌĂяo��
	void OnCollision();

	/// <summary>
	/// �e�q�Z�b�^�[
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransfom* parent);

	// �|�W�V�����Q�b�^�[
	const KMyMath::Vector3& GetPosition() const;

	// ���[���h�|�W�V�����Q�b�^�[
	const KMyMath::Vector3 GetWorldPos() const;

	// ��]�Q�b�^�[
	const KMyMath::Vector3 GetRot() const;

	// ����ł邩
	const bool GetIsDead() const;

	// ���S���o���I�������
	const bool GetIsFallEffectEnd() const;

public:
	// �Q�b�^�[
	static const KMyMath::Vector2& GetRotLimit()
	{ 
		return rotLimit;
	}

	static const float GetMoveSpeed() 
	{ 
		return moveSpeed; 
	}

	static const KMyMath::Vector2& GetPosLimitMax() 
	{ 
		return posLimitMax; 
	}

	static const KMyMath::Vector2& GetPosLimitMin() 
	{ 
		return posLimitMin; 
	}

	const bool GetIsInvisible() const
	{
		return isInvisible;
	}

	void SetPos(const KMyMath::Vector3 pos_);

private:
	// �ړ�
	void Move();

	// ��]
	void Rot();

	// �U��
	void Attack();

	// ���S���o
	void DeadEffect();

	// HP���o
	void HPEffect();

	// �_���[�W�G�t�F�N�g
	void DamageEffect();

	// �f�o�b�O�p
	void Debug();

private:
#pragma region �匳�̕ϐ�
	// Input
	KInput* input = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	KModel* model = nullptr;

	// �p�C�v���C��
	KGPlin* objPipeline;
	KGPlin* spritePipeline;
#pragma endregion

#pragma region ��]�p�ϐ�
	// ��]�߂��C�[�W���O�p
	KMyMath::Vector2 oldRot;
	float rotEaseTimer = 0;
	bool isRotEase = false;

	// Z��
	bool isRotZRight = false;
	float swayZ = 0.0f;
#pragma endregion

#pragma region ���e�B�N��
	// 2D�ɕϊ����邽�߂�3D���e�B�N��
	std::unique_ptr<Reticle3D> reticle3d = nullptr;
	// �ŏI�I�ȃ��e�B�N��
	std::unique_ptr<Reticle2D> reticle2d = nullptr;
#pragma endregion

#pragma region ����
	// �����X�s�[�h
	static const float moveSpeed;

	// �p�x���
	static const KMyMath::Vector2 rotLimit;

	// �ړ����
	static const KMyMath::Vector2 posLimitMax;
	static const KMyMath::Vector2 posLimitMin;
#pragma endregion

#pragma region HP�֘A
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

	// ���S���Ă��邩
	bool isDead;

	// HP
	std::unique_ptr<Sprite> HPUI = nullptr;
	TextureData hpTex;
	std::unique_ptr<Sprite> HPrectUI = nullptr;

	// HP�o�[
	std::unique_ptr<Sprite> HPBarUI = nullptr;
	TextureData hpbarTex;

	// ���G����
	uint32_t invisibleTimer = 0;
	const uint32_t invisibleTime = 30;
	bool isInvisible = false;

	KMyMath::Vector4 hpColor = { 0,0,0,1.0f };
#pragma endregion

#pragma region ��e���o
	bool isDamageEffect = false;
	std::unique_ptr<Sprite> damage = nullptr;
	TextureData damageTex;
	float dAlpha = 0;
#pragma endregion

#pragma region �ė��p�ϐ�
	// ���o����
	float fallEffectTime = 90;
	float fallEffectTimer = 0;

	// �����N�[���^�C��
	float expTime = 10;
	float expTimer = 0;

	bool isFallEffectEnd = false;
#pragma endregion

};