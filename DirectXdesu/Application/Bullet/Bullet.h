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
	virtual void Init(KModel* model_, KGPlin* pipeline_) = 0;

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

	// �Z�b�^�[
	virtual void SetIsDead(bool isDead_)
	{
		isDead = isDead_;
	}

	// �Փˎ��ɌĂяo��
	virtual void OnCollision();

	// ���[���h�|�W�V�����Q�b�^�[
	virtual KMyMath::Vector3 GetWorldPos();

protected:
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	KModel* model;

	// �p�C�v���C��
	KGPlin* pipeline;

	// �V�F�[�_�[
	KShader shader;

	// �X�s�[�h
	float speed = 1.0f;

	bool isDead = false;

	KMyMath::Vector3 vec;

	float lifeTimer = 0.0f;
};