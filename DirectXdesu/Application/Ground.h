#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"

class Ground
{
public:
	// ������
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos);

	// �`��
	void Draw();

	const KMyMath::Vector3 GetPos(size_t num) const;

private:
	// �I�u�W�F�N�g
	std::array<std::unique_ptr<KObject3d>,2> object3d;

	// ���f��
	std::unique_ptr<KModel> model;

	// �e�N�X�`��
	TextureData texData;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> pipeline;

	// �V�F�[�_�[
	KShader shader;
};