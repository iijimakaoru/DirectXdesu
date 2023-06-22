#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	// ������
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera_"></param>
	void Update(Camera* camera_);

	// �`��
	void Draw();

private:
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	std::unique_ptr<KModel> model;

	// �e�N�X�`��
	KTexture tex;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> pipeline;

	// �V�F�[�_�[
	KShader shader;
};