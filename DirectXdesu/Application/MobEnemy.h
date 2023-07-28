#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"

class Player;

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

	// ���S����
	virtual const bool GetIsDead()const
	{
		return isDead;
	}

	// ���[���h�|�W�V�����Q�b�^�[
	virtual const KMyMath::Vector3 GetWorldPos() const;

	// �v���C���[�|�C���^�Z�b�^�[
	virtual void SetPlayer(Player* player_) { player = player_; }

protected:
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	KModel* model = nullptr;

	// ���S�t���O
	bool isDead = false;

	// �p�C�v���C��
	KGPlin* pipeline;

	// �e�N�[���^�C��
	float coolTimer;

	// ���L����
	Player* player = nullptr;
};