#include "SceneFactory.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	// 次のシーン生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = new TitleScene();
	} else if (sceneName == "GAME") {
		newScene = new GameScene();
	} else if (sceneName == "CLEAR") {
		newScene = new ClearScene();
	} else if (sceneName == "OVER") {
		newScene = new GameOverScene();
	}

	return newScene;
}
