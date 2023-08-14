#pragma once
#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"

class GameOverScene : public BaseScene
{
public:
	GameOverScene() {};
	~GameOverScene();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Final()override;

private:
	// �C���v�b�g
	KInput* input = nullptr;

	// �J����
	std::unique_ptr<Camera> camera = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> spritePipeline = nullptr;

	// �V�F�[�_�[
	KShader spriteShader;

	// �N���A�X�v���C�g
	std::unique_ptr<Sprite> over = nullptr;
	TextureData overTex;

	// ���̃V�[���ւ̃L�[
	std::unique_ptr<Sprite> pushA = nullptr;
	TextureData pushATex;
};

