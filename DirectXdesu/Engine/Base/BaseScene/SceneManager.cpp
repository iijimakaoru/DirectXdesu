#include "SceneManager.h"
#include "SceneFactory.h"
#include "SceneChange.h"

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

			delete scene;

			SceneChange::GetInstance()->End();
		}

		// �V�[���؂�ւ�
		scene = nextScene;
		nextScene = nullptr;

		// �V�[���}�l�[�W���[���Z�b�g
		scene->SetSceneManager(this);

		// ���\�[�X�ǂݍ���
		scene->LoadResources();

		// �V�[��������
		scene->Init();
	}

	// ���s���̃V�[���X�V
	scene->Update();

	SceneChange::GetInstance()->Update();
}

void SceneManager::Draw()
{
	scene->ObjDraw();

	scene->SpriteDraw();

	SceneChange::GetInstance()->Draw();
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
