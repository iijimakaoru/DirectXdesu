#include "KMyGame.h"
#include "SceneFactory.h"

void KMyGame::Init()
{
	Framework::Init();

	// �V�[���t�@�N�g���[�𐶐�
	sceneFactory = new SceneFactory();
	// �V�[���}�l�[�W���[�ɃZ�b�g
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	// �ŏ��̃V�[��
	SceneManager::GetInstance()->ChangeScene("TITLE");
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
