#pragma once
#include "MobEnemy.h"

class CanonEnemy : public MobEnemy
{
public:
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="model_">�g�p���郂�f��</param>
	/// <param name="pos">�����ʒu</param>
	/// <returns></returns>
	static CanonEnemy* Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model_"></param>
	void Init(KModel* model_, KGPlin* pipeline_) override;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	/// <param name="cameraPos"></param>
	void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �U��
	/// </summary>
	void Attack() override;

	/// <summary>
	/// �o��
	/// </summary>
	void Appear();

private:
	/// <summary>
	/// �X�^�[�g�|�W�V����
	/// </summary>
	/// <param name="startPos_"></param>
	void SetStartPos(const KMyMath::Vector3& startPos_);

private:
	// �o�����o�����ǂ���
	bool isAppear = false;

	// �C�[�W���O�^�C�}�[
	float easeTimer = 0;

	// ���
	const float easeTime = 30;

	// �ŏ��̃X�P�[��
	KMyMath::Vector3 startPos;
};

