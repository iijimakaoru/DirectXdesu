#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"

class SkyBox
{
public:
	static SkyBox* Create(KModel* model_, KGPlin* objPipeline_, const float posZ);

public:
	// ������
	void Init(KModel* model_, KGPlin* objPipeline_, const float posZ);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera_"></param>
	void Update(ViewProjection* viewPro);

	// �`��
	// �I�u�W�F�N�g
	void ObjDraw();

	/// <summary>
	/// z���W�Z�b�^�[
	/// </summary>
	/// <param name="posZ"></param>
	void SetPosZ(const float posZ);

private:
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	KModel* model = nullptr;

	// �p�C�v���C��
	KGPlin* objPipeline;
};

