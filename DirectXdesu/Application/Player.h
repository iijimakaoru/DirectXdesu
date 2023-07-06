#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"
#include "KInput.h"
#include "Bullet.h"

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	// ������
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera_"></param>
	void Update(ViewProjection* viewPro);

	// �ړ�
	void Move();

	// ��]
	void Rot();

	// �U��
	void Attack();

	// �`��
	void Draw();

	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransfom* parent);

	// �|�W�V�����Q�b�^�[
	KMyMath::Vector3& GetPosition()
	{
		return object3d->transform.pos;
	}

	KMyMath::Vector3 GetWorldPos()
	{
		// ���[���h���W�i�[�ϐ�
		KMyMath::Vector3 result;

		// ���[���h�s��̕��s�ړ������擾
		result.x = object3d->transform.matWorld.m[3][0];
		result.y = object3d->transform.matWorld.m[3][1];
		result.z = object3d->transform.matWorld.m[3][2];

		return result;
	}

private:
	// Input
	KInput* input = nullptr;
	// �p�b�h�̓��͏��
	KMyMath::Vector2 leftStickPos;

	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	std::unique_ptr<KModel> model;

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

	// �����X�s�[�h
	float moveSpeed = 0.5f;

	// �p�x���
	const KMyMath::Vector2 rotLimit = { 35.0f, 25.0f };

	// �e
	std::array<std::unique_ptr<Bullet>, 60> bullet = { nullptr };
	KMyMath::Vector3 bulletVec;
};