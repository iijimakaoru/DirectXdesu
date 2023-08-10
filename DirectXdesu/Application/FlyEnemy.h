#pragma once
#include "MobEnemy.h"
class FlyEnemy : public MobEnemy
{
public:
	static FlyEnemy* Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos, const KMyMath::Vector2& endPos);

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
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �o��
	/// </summary>
	void Appear();

private:
	// �C�[�W���O�^�C�}�[
	float easeTimer = 0;

	// ���
	const float easeTime = 30;

	// �ŏ��̃X�P�[��
	KMyMath::Vector3 startScale;

	// �ŏI�X�P�[��
	KMyMath::Vector3 endScale;

	// �o�����o�����ǂ���
	bool isAppear = false;
};

