#pragma once
#include "Sprite.h"

class Warning
{
public:
	void Init();

	void Update();

	void Draw();

	bool GetIsDelete() { return isDelete; }

	int32_t GetTime() { return warningTime; }

private:
	// �X�v���C�g
	std::unique_ptr<Sprite> warning = nullptr;

	// �e�N�X�`��
	TextureData warningTex;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> pipeline = nullptr;

	// �V�F�[�_�[
	KShader shader;

	// �x������
	int32_t warningTimer = 0;
	int32_t warningTime = 0;

	// �A���t�@�l
	float alpha = 0.0f;

	// �A���t�@�l�ύX�X�s�[�h
	float alphaSpeed = -0.03f;

	// �폜�t���O
	bool isDelete = false;
};

