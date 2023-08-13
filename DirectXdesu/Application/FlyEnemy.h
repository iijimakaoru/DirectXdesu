#pragma once
#include "MobEnemy.h"
class FlyEnemy : public MobEnemy
{
public:
	static FlyEnemy* Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& startPos_, const KMyMath::Vector2& endPos_, const float& speed_);

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

	/// <summary>
	/// �G���h�|�W�V����
	/// </summary>
	/// <param name="endPos_"></param>
	void SetEndPos(const KMyMath::Vector2& endPos_);

	/// <summary>
	/// �X�s�[�h�Z�b�g
	/// </summary>
	/// <param name="speed"></param>
	void SetSpeed(const float& speed);

private:
	// �C�[�W���O�^�C�}�[
	float easeTimer = 0;

	// ���
	const float easeTime = 30;

	// �ŏ��̃X�P�[��
	KMyMath::Vector3 startPos;

	// �ŏI�X�P�[��
	KMyMath::Vector2 endPos;

	// �o�����o�����ǂ���
	bool isAppear = false;

	// �X�s�[�h
	float moveSpeed = 0;
};

