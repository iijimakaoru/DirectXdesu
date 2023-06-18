#include "SceneManager.h"
#include "SceneFactory.h"

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

void SceneManager::Final()
{
	scene->Final();
	scene = nullptr;

	delete scene;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	// ���̃V�[���𐶐�
	nextScene = sceneFactory->CreateScene(sceneName);
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}
