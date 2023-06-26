#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"
#include "KInput.h"

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
	void Update(ViewProjection& viewPro);

	// �ړ�
	void Move();

	// ��]
	void Rot();

	// �`��
	void Draw();

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
};