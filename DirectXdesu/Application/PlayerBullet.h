#pragma once
#include "Bullet.h"

class PlayerBullet : public Bullet
{
public:
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="pipeline_">�p�C�v���C��</param>
	/// <param name="pos">�����ʒu</param>
	/// <param name="vec_">��������</param>
	/// <param name="rot_">��]�p</param>
	/// <param name="bulletSpeed">�e��</param>
	/// <returns></returns>
	static PlayerBullet* Create(KModel* model, KGPlin* pipeline_,
		const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed);

public:
	// ������
	void Init(KModel* model_, KGPlin* pipeline_) override;
};