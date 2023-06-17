#include "SceneManager.h"

void SceneManager::Update()
{
	// �V�[���؂�ւ�
	if (nextScene)
	{
		// ���V�[���I��
		if (scene)
		{
			scene->Final();
			delete scene;
		}

		// �V�[���؂�ւ�
		scene = nextScene;
		nextScene = nullptr;

		// �V�[���}�l�[�W���[���Z�b�g
		scene->SetSceneManager(this);

		// �V�[��������
		scene->Init();
	}

	// ���s���̃V�[���X�V
	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}

void SceneManager::Final()
{
	scene->Final();
	delete scene;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}
