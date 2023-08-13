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
	enum EnemysType
	{
		None,
		Fly,
		Canon,
		Appear
	};

	// ������
	virtual void Init(KModel* model_, KGPlin* pipeline_);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro,const KMyMath::Vector3& cameraPos);

	// �`��
	virtual void Draw();

	// �Փˎ��ɌĂяo��
	virtual void OnCollision();

	// ���S����
	virtual const bool GetIsDead()const
	{
		return isDead;
	}

	/// <summary>
	/// �U��
	/// </summary>
	virtual void Attack();

	// ���[���h�|�W�V�����Q�b�^�[
	virtual const KMyMath::Vector3 GetWorldPos() const;

	// �v���C���[�|�C���^�Z�b�^�[
	virtual void SetPlayer(Player* player_) { player = player_; }

protected:
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���S�t���O
	bool isDead = false;

	// �N�[���^�C��
	float coolTimer = 0;
	const float coolTime = 120;

	// ���L����
	Player* player = nullptr;

private:
	// ���f��
	KModel* model = nullptr;

	// �p�C�v���C��
	KGPlin* pipeline;
};