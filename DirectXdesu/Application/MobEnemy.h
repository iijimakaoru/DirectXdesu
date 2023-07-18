#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"

class MobEnemy
{
public:
	// ������
	void Init(KModel* model_);

	// ���z�u
	void Set(KMyMath::Vector3& pos);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro,const KMyMath::Vector3& cameraPos);

	// �`��
	void Draw();

	// �Փˎ��ɌĂяo��
	void OnCollision();

	const bool GetIsDead()const
	{
		return isDead;
	}

	// ���[���h�|�W�V�����Q�b�^�[
	KMyMath::Vector3 GetWorldPos();

private:
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	KModel* model = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> pipeline;

	// �V�F�[�_�[
	KShader shader;

	// �X�s�[�h
	float speed = 0.5f;

	bool isDead = false;
};