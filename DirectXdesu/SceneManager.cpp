#include "SceneManager.h"

SceneManager::~SceneManager()
{
	scene->Final();
	delete scene;
}

void SceneManager::Update()
{
	// �V�[���؂�ւ�
	if (nextScene)
	{
		// ���V�[���I��
		if (scene)
		{
			scene->Final();
			scene = nullptr;
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
