#include "SceneManager.h"
#include "SceneFactory.h"

void SceneManager::Update()
{
	// シーン切り替え
	if (nextScene)
	{
		// 旧シーン終了
		if (scene)
		{
			scene->Final();
			scene = nullptr;
		}

		// シーン切り替え
		scene = nextScene;
		nextScene = nullptr;

		// シーンマネージャーをセット
		scene->SetSceneManager(this);

		// シーン初期化
		scene->Init();
	}

	// 実行中のシーン更新
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

	// 次のシーンを生成
	nextScene = sceneFactory->CreateScene(sceneName);
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}
