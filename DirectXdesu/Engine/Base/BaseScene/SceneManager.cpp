#include "SceneManager.h"
#include "SceneFactory.h"
#include "SceneChange.h"

void SceneManager::Update()
{
	// シーンチェンジの更新
	SceneChange::GetInstance()->Update();

	// シーン切り替え
	if (nextScene)
	{
		// 旧シーン終了
		if (scene)
		{
			scene->Final();
			scene = nullptr;

			delete scene;

			SceneChange::GetInstance()->SceneChangeEnd();
		}

		// シーン切り替え
		scene = nextScene;
		nextScene = nullptr;

		// シーンマネージャーをセット
		scene->SetSceneManager(this);

		// リソース読み込み
		scene->LoadResources();

		// シーン初期化
		scene->Init();
	}

	// 実行中のシーン更新
	scene->Update();
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

	// 次のシーンを生成
	nextScene = sceneFactory->CreateScene(sceneName);
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}
