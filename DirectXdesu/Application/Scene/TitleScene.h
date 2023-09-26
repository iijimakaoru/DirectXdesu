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

	KMyMath::Vector2 titlePos = { 0,0 };

	// ���̃V�[���ւ̃L�[
	std::unique_ptr<Sprite> pushA = nullptr;
	TextureData pushATex;

	KMyMath::Vector2 pushAPos = { 0,0 };

	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d;

	// ���f��
	std::unique_ptr<KModel> model = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> objPipeline;
	KShader objShader;

	float nowAngle = 0;

	float easeTimer = 0;

	const float easeTime = 30;

	bool goGame = false;

	float texEaseTimer = 0;

	const float texEaseTime = 10;

	float objEaseTimer = 0;

	const float objEaseTime = 15;

	uint32_t phase = 0;

	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
};

