#pragma once
#include"BaseScene.h"
#include"AbstractSceneFactory.h"

//シーン管理
class SceneManager
{
private:

	
	std::unique_ptr<BaseScene> scene;
	std::unique_ptr<BaseScene> nextScene;

	AbstractSceneFactory* sceneFactory = nullptr;

	std::string sceneName;
	std::string nextSceneName;

public:

	//更新処理
	void Update();

	//描画処理
	void Draw();

	//終了処理
	void Finalize();

	void ChangeScene(const std::string& SceneName);

	void SetSceneFactory(AbstractSceneFactory* SceneFactory);

	std::string GetSceneName();

	//シングルトン
	static SceneManager* GetInstance();

private:

	SceneManager() = default;
	~SceneManager() = default;

	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(const SceneManager&) = delete;
};