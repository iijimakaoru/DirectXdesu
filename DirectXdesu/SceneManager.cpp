#include "SceneManager.h"

SceneManager::~SceneManager()
{
	scene->Final();
	delete scene;
}

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
