#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "ViewProjection.h"
#include "TextureManager.h"

class BossEnemy
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="pipeline_"></param>
	/// <param name="initPos"></param>
	/// <param name="HP"></param>
	virtual void Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos, const float HP) = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	virtual void Update(ViewProjection* viewPro) = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ���[���h���W�Q�b�^�[
	/// </summary>
	/// <returns></returns>
	virtual const KMyMath::Vector3 GetWorldPos() const;

	// �����������̏���
	virtual void OnCollision();

protected:
	// �g�����X�t�H�[��
	std::unique_ptr<KObject3d> object3d = nullptr;

	// ���f��
	KModel* model = nullptr;

	// �p�C�v���C��
	KGPlin* pipeline = nullptr;

	// �̗�
	float HP = 0;
};

