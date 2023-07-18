#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
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
	// ������
	void Init(KModel* model_);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera_"></param>
	void Update(ViewProjection* viewPro);

	// �`��
	void Draw();

	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransfom* parent);

	// �|�W�V�����Q�b�^�[
	const KMyMath::Vector3& GetPosition() const
	{
		return object3d->transform.pos;
	}

	// ���[���h�|�W�V�����Q�b�^�[
	KMyMath::Vector3 GetWorldPos();

	// ��]�Q�b�^�[
	const KMyMath::Vector3 GetRot() const
	{
		return object3d->transform.rot;
	}

	// �Փˎ��ɌĂяo��
	void OnCollision();

	// �e�̃��X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() 
	{ 
		return bullets; 
	}

	// �Q�b�^�[
	static const KMyMath::Vector2& GetRotLimit() { return rotLimit; }
	static const float GetMoveSpeed() { return moveSpeed; }
	static const KMyMath::Vector2& GetPosLimitMax() { return posLimitMax; }
	static const KMyMath::Vector2& GetPosLimitMin() { return posLimitMin; }
private:
	// �ړ�
	void Move();

	// ��]
	void Rot();

	// �U��
	void Attack();

private:
	// Input
	KInput* input = nullptr;
	// �p�b�h�̓��͏��
	KMyMath::Vector2 leftStickPos;

	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	KModel* model = nullptr;

	// �e�N�X�`��
	KTexture tex;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> pipeline;

	// �V�F�[�_�[
	KShader shader;

	// �X�s�[�h
	float speed = 1.0f;

	// ��]�߂��C�[�W���O�p
	KMyMath::Vector2 oldRot;
	float rotEaseTimer = 0;
	bool isRotEase = false;

	// Z��
	bool isRotZRight = false;
	float swayZ = 0.0f;

	// �e
	std::list<std::unique_ptr<PlayerBullet>> bullets;

	// ���e�B�N��
	std::unique_ptr<Reticle3D> reticle3d = nullptr;
	std::unique_ptr<Reticle2D> reticle2d = nullptr;

	// �����X�s�[�h
	static const float moveSpeed; // = 0.16f

	// �p�x���
	static const KMyMath::Vector2 rotLimit; // = { 35.0f, 25.0f };

	// �ړ����
	static const KMyMath::Vector2 posLimitMax; // = { 45.0f, 25.0f };
	static const KMyMath::Vector2 posLimitMin; // = { 45.0f, 25.0f };
};