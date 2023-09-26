#pragma once
#include "Camera.h"
#include "KGPlin.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"

class TitleScene : public BaseScene
{
public:
	TitleScene(){};
	~TitleScene();
	void LoadResources()override;
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

	// �^�C�g���X�v���C�g
	std::unique_ptr<Sprite> titleName = nullptr;
	TextureData titleTex;

	// ���̃V�[���ւ̃L�[
	std::unique_ptr<Sprite> pushA = nullptr;
	TextureData pushATex;

	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	std::unique_ptr<KModel> model = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> objPipeline;
	KShader objShader;

	float nowAngle = 0;

	float easeTimer = 0;

	float easeTime = 30;

	bool goGame = false;
};

