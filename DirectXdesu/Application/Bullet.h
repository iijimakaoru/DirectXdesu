#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"
#include "TextureManager.h"

class Bullet
{
public:
	// ������
	virtual void Init() = 0;

	/// <summary>
	/// �Ăяo��
	/// </summary>
	/// <param name="pos"></param>
	virtual void Set(const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro);

	// �`��
	virtual void Draw();

	// �Q�b�^�[
	virtual bool GetIsDead() const
	{
		return isDead;
	}

	// �Փˎ��ɌĂяo��
	virtual void OnCollision();

	// ���[���h�|�W�V�����Q�b�^�[
	virtual KMyMath::Vector3 GetWorldPos();

protected:
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	std::unique_ptr<KModel> model;

	// �e�N�X�`��
	TextureData texData;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> pipeline;

	// �V�F�[�_�[
	KShader shader;

	// �X�s�[�h
	float speed = 1.0f;

	bool isDead = false;

	KMyMath::Vector3 vec;

	float lifeTimer = 0.0f;
};