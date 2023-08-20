#pragma once
#include "Sprite.h"

class SceneChange
{
public:
	void Init();

	void Update();

	void Draw();

public:
	static SceneChange* GetInstance();

	void Delete();

private:
	SceneChange() = default;
	~SceneChange() = default;
	SceneChange(const SceneChange&) = delete;
	const SceneChange& operator=(const SceneChange&) = delete;

private:
	// �X�v���C�g
	std::unique_ptr<Sprite> black = nullptr;

	// �e�N�X�`��
	TextureData blackTex;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> pipeline = nullptr;

	// �V�F�[�_�[
	KShader shader;

	// �Ó]����
	int32_t inTimer = 0;
	int32_t inTime = 30;
	bool isIn = false;

	// ��������
	int32_t outTimer = 0;
	int32_t outTime = 30;
	bool isOut = false;

	// �A���t�@�l
	float alpha = 0.0f;

	// �폜�t���O
	bool isChange = false;

private:
	static SceneChange* sceneChange;
};

