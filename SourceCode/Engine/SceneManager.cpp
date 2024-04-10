#include "SceneManager.h"
#include<cassert>

void SceneManager::Update()
{
	//次シーンの予約があるなら
	if (nextScene)
	{
		//旧シーン終了
		if (scene)
		{
			scene->Finalize();
			scene.reset();
		}

		//シーン切り替え
		scene = std::move(nextScene);

		//次シーンを初期化する
		scene->Initialize();

		//シーンの名前を変更
		sceneName = nextSceneName;
	}

	scene->Update();
}
void SceneManager::Draw()
{
	scene->Draw();
}

void SceneManager::Finalize()
{
	scene->Finalize();
	scene.reset();
}

void SceneManager::ChangeScene(const std::string& SceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	//次シーンを生成
	nextScene = sceneFactory->CreateScene(SceneName);
	nextSceneName = SceneName;
}

void SceneManager::SetSceneFactory(AbstractSceneFactory* SceneFactory)
{
	sceneFactory = SceneFactory;
}

std::string SceneManager::GetSceneName()
{
	return sceneName;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}