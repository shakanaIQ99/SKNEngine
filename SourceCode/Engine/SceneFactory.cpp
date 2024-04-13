#include "SceneFactory.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"TutorialScene.h"
#include"ResultScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    //次のシーンを生成
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE")
    {
        newScene = new TitleScene();
    }
    else if (sceneName == "GAME")
    {
        newScene = new GameScene();
    }
    else if (sceneName == "RESULT")
    {
        newScene = new ResultScene();
    }
    else if (sceneName == "TUTORIAL")
    {
        newScene = new TutorialScene();
    }

    return std::unique_ptr<BaseScene>(newScene);
}
