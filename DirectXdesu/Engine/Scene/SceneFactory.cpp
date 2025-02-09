#include "SceneFactory.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SettingScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	// 次のシーン生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = new TitleScene();
	} else if (sceneName == "GAME") {
		newScene = new GameScene();
	} else if (sceneName == "SETTING")
	{
		newScene = new SettingScene();
	}

	return newScene;
}
