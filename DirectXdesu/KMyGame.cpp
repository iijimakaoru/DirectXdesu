#include "KMyGame.h"
#include "GameScence.h"
#include "TitleScene.h"

void KMyGame::Init()
{
	Framework::Init();

	// �ŏ��̃V�[���쐬
	BaseScene* scene = new TitleScene();
	// �V�[���}�l�[�W���[�ɃV�[���Z�b�g
	sceneManager->SetNestScene(scene);
}

void KMyGame::Update()
{
	Framework::Update();
}

void KMyGame::Draw()
{
	// �`��J�n
	dx->PreDraw();

	// �V�[���}�l�[�W���[�̕`��
	sceneManager->Draw();

	// Imgui�`��
	imguiMane.Draw();

	dx->PostDraw();
}

void KMyGame::Final()
{
	Framework::Final();
}
