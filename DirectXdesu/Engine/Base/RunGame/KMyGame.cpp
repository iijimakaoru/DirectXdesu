#include "KMyGame.h"
#include "SceneFactory.h"
#include "BulletManager.h"

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
	// �����_�[�e�N�X�`���ւ̕`��
	postEffect->PreDrawScene();

	// �V�[���}�l�[�W���[�̕`��
	sceneManager->Draw();

	postEffect->PostDrawScene();

	// �`��J�n
	dx->PreDraw();

	// �|�X�g�G�t�F�N�g�`��
	postEffect->Draw();

	// Imgui�`��
#ifdef _DEBUG
	imguiMane.Draw();
#endif 

	dx->PostDraw();
}

void KMyGame::Final()
{
	Framework::Final();

	BulletManager::GetInstance()->Delete();
}
