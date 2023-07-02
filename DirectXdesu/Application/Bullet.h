#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"

class Bullet
{
public:
	// ������
	void Init();

	/// <summary>
	/// �Ăяo��
	/// </summary>
	/// <param name="pos"></param>
	void Set(const KMyMath::Vector3 pos, const KMyMath::Vector3 vec);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection& viewPro);

	// �`��
	void Draw();

	// �Q�b�^�[
	bool GetIsDead()
	{
		return isDead;
	}

private:
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

	bool isDead = true;

	KMyMath::Vector3 vec;

	float lifeTimer = 0.0f;
};