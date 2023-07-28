#pragma once
#include "MobEnemy.h"

class AppearEnemy : public MobEnemy
{
public:
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="model_">�g�p���郂�f��</param>
	/// <param name="pos">�����ʒu</param>
	/// <returns></returns>
	static AppearEnemy* Create(KModel* model_,KGPlin* pipeline_, const KMyMath::Vector3& pos);

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
};

