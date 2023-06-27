#pragma once
#include <memory>
#include "KTexture.h"
#include "KObject3d.h"
#include "KModel.h"

class Bullet
{
public:
	// ������
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection& viewPro);

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

	// �X�s�[�h
	float speed = 1.0f;
};