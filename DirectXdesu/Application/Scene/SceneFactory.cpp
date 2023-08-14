#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScence.h"
#include "ClearScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // éüÇÃÉVÅ[Éìê∂ê¨
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE")
    {
        newScene = new TitleScene();
    }
    else if (sceneName == "GAME")
    {
        newScene = new GameScence();
    }
    else if (sceneName == "CLEAR")
    {
        newScene = new ClearScene();
    }

    return newScene;
}
