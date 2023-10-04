#pragma once
#include "Camera.h"
#include "KGPlin.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"
#include "TitleCamera.h"

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

	// �^�C�g���������o
	void StartScene();

	// ���̃V�[����
	void GoNextScene();
private:
	// �C���v�b�g
	KInput* input = nullptr;

	// �J����
	std::unique_ptr<TitleCamera> camera = nullptr;

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

#pragma region �v���C���[�@��
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object3d = nullptr;

	// ���f��
	std::unique_ptr<KModel> model = nullptr;
#pragma endregion

#pragma region �V��
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> skyDome = nullptr;

	// ���f��
	std::unique_ptr<KModel> skyDomeModel = nullptr;
#pragma endregion

	// �p�C�v���C��
	std::unique_ptr<KGPlin> objPipeline;
	KShader objShader;

	// ��ʃT�C�Y
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

#pragma region �^�C�g���n�߂̉��o�ϐ�
	// �V�[���n�܂����t���O
	bool startScene = false;

	// ���o�̃t�F�[�Y
	uint32_t startScenePhase = 0;

	// �x�W�G�Ȑ��P
	KMyMath::Vector3 start = { 0,-30,180 };
	KMyMath::Vector3 p1 = { 0,-30,20 };
	KMyMath::Vector3 p2 = { 0,-30,-40 };
	KMyMath::Vector3 end = { 0,60,-60 };

	// �I�u�W�F�N�g�g��^�C�}�[
	float objEaseTimer = 0;
	const float objEaseTime = 20;

	// �t���b�V��
	std::unique_ptr<Sprite> flash = nullptr;
	TextureData flashTex;

	float flashAlpha = 0;
#pragma endregion

#pragma region ���̃V�[���ւ̈ڍs���o�ϐ�
	// ���̃V�[���ւ����t���O
	bool goGame = false;

	// ���o�̃t�F�[�Y
	uint32_t goGamePhase = 0;

	// �e�L�X�g������΂��^�C�}�[
	float texEaseTimer = 0;
	const float texEaseTime = 10;
#pragma endregion

	// �t�F�[�Y����
	float phaseTimer = 0;
	float phaseTime = 0;
};

