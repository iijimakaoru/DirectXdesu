#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"

class MobEnemy
{
public:
	enum EnemysKind
	{
		None,
		Battery,
		Fall,
		Appear
	};

	// ������
	virtual void Init(KModel* model_, KGPlin* pipeline_) = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro,const KMyMath::Vector3& cameraPos) = 0;

	// �`��
	virtual void Draw();

	// �Փˎ��ɌĂяo��
	virtual void OnCollision();

	virtual const bool GetIsDead()const
	{
		return isDead;
	}

	// ���[���h�|�W�V�����Q�b�^�[
	virtual const KMyMath::Vector3 GetWorldPos() const;

protected:
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	KModel* model = nullptr;

	// ���S�t���O
	bool isDead = false;

	// �p�C�v���C��
	KGPlin* pipeline;

	// �X�s�[�h
	float speed = 0.5f;
};