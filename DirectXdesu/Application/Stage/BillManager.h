#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"
#include "Bill1.h"

class BillManager
{
public:
	// ������
	void Init();

	// �X�V
	void Update(ViewProjection* viewPro, const float& cameraZ);

	// �`��
	void Draw();

	// �r���Z�b�g
	void Set(const float& cameraZ);
	void RightSet(const float cameraZ);
	void LeftSet(const float cameraZ);

private:
	// ���f��
	std::unique_ptr<KModel> model = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> objPipeline;

	// �V�F�[�_�[
	KShader objShader;

	// �r���P
	std::list<std::unique_ptr<Bill1>> bill1s;

	// �^�C�}�[
	int32_t billtimer = 0;
	const int32_t billtime = 120;
};

