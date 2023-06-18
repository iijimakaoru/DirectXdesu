#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScence.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // 次のシーン生成
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE")
    {
        newScene = new TitleScene();
    }
    else if (sceneName == "GAME")
    {
        newScene = new GameScence();
    }

    return newScene;
}
